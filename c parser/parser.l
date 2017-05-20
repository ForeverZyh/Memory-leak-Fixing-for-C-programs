%e  1019
%p  2807
%n  371
%k  284
%a  1213
%o  1117

O   [0-7]
D   [0-9]
NZ  [1-9]
L   [a-zA-Z_]
A   [a-zA-Z_0-9]
H   [a-fA-F0-9]
HP  (0[xX])
E   ([Ee][+-]?{D}+)
P   ([Pp][+-]?{D}+)
FS  (f|F|l|L)
IS  (((u|U)(l|L|ll|LL)?)|((l|L|ll|LL)(u|U)?))
CP  (u|U|L)
SP  (u8|u|U|L)
ES  (\\(['"\?\\abfnrtv]|[0-7]{1,3}|x[a-fA-F0-9]+))
WS  [ \t\v\n\f]

%{
#include "myheader.h"
#define YYSTYPE myYYSTYPE
#include "y.tab.h"
#include "cfg.h"
#include "point_analysis.h"
int sym_table[MAXN];
vector<node*> forest;
int cnt;
map<string,int> string_to_int;
vector<func> fun;
int total_string=0;
vector<string> int_to_string;
int line=1;
int malloc_cnt=0;
vector<pair<CFGnode*,CFGnode*> > ext_dec;

static int sym_type(const char * s)  /* returns type from symbol table */
{
	for(int i=0;i<cnt;i++)
		if (s==int_to_string[sym_table[i]]) 
        {
            //printf("\ncheck! %s\n",s);
            return TYPEDEF_NAME;
        }
	return IDENTIFIER;
}

node::node()
{
    ln=line;
    str="";tag=0;fa=NULL;
    son.clear();
}

node::node(const char *s)
{
    ln=line;
    str=s;tag=0;fa=NULL;
    son.clear();
}

//#define sym_type(identifier) IDENTIFIER /* with no symbol table, fake it */

static void comment(void);
static int check_type(void);
void count();
%}

%%
"/*"                                    { count();comment(); }
"//".*                                    { count();/* consume //-comment */ }

"auto"					{ count(); yylval=new node("AUTO"); return(AUTO); }
"break"					{ count(); yylval=new node("BREAK"); return(BREAK); }
"case"					{ count(); yylval=new node("CASE"); return(CASE); }
"char"					{ count(); yylval=new node("CHAR"); return(CHAR); }
"const"					{ count(); yylval=new node("CONST"); return(CONST); }
"continue"				{ count(); yylval=new node("CONTINUE"); return(CONTINUE); }
"default"				{ count(); yylval=new node("DEFAULT"); return(DEFAULT); }
"do"					{ count(); yylval=new node("DO"); return(DO); }
"double"				{ count(); yylval=new node("DOUBLE"); return(DOUBLE); }
"else"					{ count(); yylval=new node("ELSE"); return(ELSE); }
"enum"					{ count(); yylval=new node("ENUM"); return(ENUM); }
"extern"				{ count(); yylval=new node("EXTERN"); return(EXTERN); }
"float"					{ count(); yylval=new node("FLOAT"); return(FLOAT); }
"for"					{ count(); yylval=new node("FOR"); return(FOR); }
"goto"					{ count(); yylval=new node("GOTO"); return(GOTO); }
"if"					{ count(); yylval=new node("IF"); return(IF); }
"inline"				{ count(); yylval=new node("INLINE"); return(INLINE); }
"int"					{ count(); yylval=new node("INT"); return(INT); }
"long"					{ count(); yylval=new node("LONG"); return(LONG); }
"register"				{ count(); yylval=new node("REGISTER"); return(REGISTER); }
"restrict"				{ count(); yylval=new node("RESTRICT"); return(RESTRICT); }
"return"				{ count(); yylval=new node("RETURN"); return(RETURN); }
"short"					{ count(); yylval=new node("SHORT"); return(SHORT); }
"signed"				{ count(); yylval=new node("SIGNED"); return(SIGNED); }
"sizeof"				{ count(); yylval=new node("SIZEOF"); return(SIZEOF); }
"static"				{ count(); yylval=new node("STATIC"); return(STATIC); }
"struct"				{ count(); yylval=new node("STRUCT"); return(STRUCT); }
"switch"				{ count(); yylval=new node("SWITCH"); return(SWITCH); }
"typedef"				{ count(); yylval=new node("TYPEDEF"); return(TYPEDEF); }
"union"					{ count(); yylval=new node("UNION"); return(UNION); }
"unsigned"				{ count(); yylval=new node("UNSIGNED"); return(UNSIGNED); }
"void"					{ count(); yylval=new node("VOID"); return(VOID); }
"volatile"				{ count(); yylval=new node("VOLATILE"); return(VOLATILE); }
"while"					{ count(); yylval=new node("WHILE"); return(WHILE); }
"_Alignas"                              { count(); return ALIGNAS; }
"_Alignof"                              { count(); return ALIGNOF; }
"_Atomic"                               { count(); return ATOMIC; }
"_Bool"                                 { count(); return BOOL; }
"_Complex"                              { count(); return COMPLEX; }
"_Generic"                              { count(); return GENERIC; }
"_Imaginary"                            { count(); return IMAGINARY; }
"_Noreturn"                             { count(); return NORETURN; }
"_Static_assert"                        { count(); return STATIC_ASSERT; }
"_Thread_local"                         { count(); return THREAD_LOCAL; }
"__func__"                              { count(); return FUNC_NAME; }

{L}{A}*					{ count(); return check_type(); }

{HP}{H}+{IS}?				{ count(); yylval=new node("I_CONSTANT"); return I_CONSTANT; }
{NZ}{D}*{IS}?				{ count(); yylval=new node("I_CONSTANT"); return I_CONSTANT; }
"0"{O}*{IS}?				{ count(); yylval=new node("I_CONSTANT"); return I_CONSTANT; }
{CP}?"'"([^'\\\n]|{ES})+"'"		{ count(); yylval=new node("I_CONSTANT"); return I_CONSTANT; }

{D}+{E}{FS}?				{ count(); yylval=new node("F_CONSTANT"); return F_CONSTANT; }
{D}*"."{D}+{E}?{FS}?			{ count(); yylval=new node("F_CONSTANT"); return F_CONSTANT; }
{D}+"."{E}?{FS}?			{ count(); yylval=new node("F_CONSTANT"); return F_CONSTANT; }
{HP}{H}+{P}{FS}?			{ count(); yylval=new node("F_CONSTANT"); return F_CONSTANT; }
{HP}{H}*"."{H}+{P}{FS}?			{ count(); yylval=new node("F_CONSTANT"); return F_CONSTANT; }
{HP}{H}+"."{P}{FS}?			{ count(); yylval=new node("F_CONSTANT"); return F_CONSTANT; }

({SP}?\"([^"\\\n]|{ES})*\"{WS}*)+	{ count(); yylval=new node("STRING_LITERAL"); return STRING_LITERAL; }

"..."					{ count(); return ELLIPSIS; }
">>="					{ count(); yylval=new node(">>="); return RIGHT_ASSIGN; }
"<<="					{ count(); yylval=new node("<<="); return LEFT_ASSIGN; }
"+="					{ count(); yylval=new node("+="); return ADD_ASSIGN; }
"-="					{ count(); yylval=new node("-="); return SUB_ASSIGN; }
"*="					{ count(); yylval=new node("*="); return MUL_ASSIGN; }
"/="					{ count(); yylval=new node("/="); return DIV_ASSIGN; }
"%="					{ count(); yylval=new node("%="); return MOD_ASSIGN; }
"&="					{ count(); yylval=new node("&="); return AND_ASSIGN; }
"^="					{ count(); yylval=new node("^="); return XOR_ASSIGN; }
"|="					{ count(); yylval=new node("|="); return OR_ASSIGN; }
">>"					{ count(); return RIGHT_OP; }
"<<"					{ count(); return LEFT_OP; }
"++"					{ count(); return INC_OP; }
"--"					{ count(); return DEC_OP; }
"->"					{ count(); return PTR_OP; }
"&&"					{ count(); return AND_OP; }
"||"					{ count(); return OR_OP; }
"<="					{ count(); return LE_OP; }
">="					{ count(); return GE_OP; }
"=="					{ count(); return EQ_OP; }
"!="					{ count(); return NE_OP; }
";"					{ count(); return ';'; }
("{"|"<%")				{ count(); yylval=new node("{"); return '{'; }
("}"|"%>")				{ count(); yylval=new node("}"); return '}'; }
","					{ count(); return ','; }
":"					{ count(); return ':'; }
"="					{ count(); return '='; }
"("					{ count(); return '('; }
")"					{ count(); return ')'; }
("["|"<:")				{ count(); return '['; }
("]"|":>")				{ count(); return ']'; }
"."					{ count(); return '.'; }
"&"					{ count(); yylval=new node("&"); return '&'; }
"!"					{ count(); return '!'; }
"~"					{ count(); return '~'; }
"-"					{ count(); return '-'; }
"+"					{ count(); return '+'; }
"*"					{ count(); yylval=new node("*"); return '*'; }
"/"					{ count(); return '/'; }
"%"					{ count(); return '%'; }
"<"					{ count(); return '<'; }
">"					{ count(); return '>'; }
"^"					{ count(); return '^'; }
"|"					{ count(); return '|'; }
"?"					{ count(); return '?'; }

{WS}+					{ count();/* whitespace separates tokens */ }
.					{ count();/* discard bad characters */ }

%%

extern "C"
{
	extern void yyerror(const char *);  /* prints grammar violation message */
}

void dfs(node* root,int cnt)
{
    for(int i=0;i<cnt;i++) printf(" ");
    printf("%s\n",root->str.c_str());
    for(int i=0;i<(int)root->son.size();i++)
        dfs(root->son[i],cnt+1);
}



int yywrap(void)        /* called at end of input */
{
    printf("%d\n",(int)forest.size());
    node *root;
    for(int i=0;i<(int)forest.size();i++)
        if (forest[i]->fa==NULL)
        {
            root=forest[i];
        }
    dfs(root,0);
    CFGnode::rac_cnt=0;
    CFGnode::flag=1;
    ext_dec.clear();
    function(root);

    printf("%d\n",fun.size());
    for(int i=0;i<fun.size();i++)
    {
        fun[i].CFG.first->print();
    }

    clone_build_graph_init();

    CFGnode *begin, *end;
    pair<CFGnode*, CFGnode*> pa = clone_build_graph_work();
    begin = pa.first, end = pa.second;
    
    for(int i=0;i<fun.size();i++)
        if (int_to_string[fun[i].id]=="main")
        {
            CFGnode::flag++;
            if (fun[i].init.first) ext_dec.push_back(fun[i].init);
            for(int j=1;j<ext_dec.size();j++)
            {
                link(ext_dec[j-1].second,ext_dec[j].first);
            }
            if (ext_dec.size()) 
            {
                link(ext_dec.back().second,fun[i].CFG.first);
                point_analysis(ext_dec[0].first);
            }
            else
                point_analysis(fun[i].CFG.first);
            CFGnode::flag++;
            printf("\n===================================\n");
            fun[i].CFG.first->print();
        }

    return 1;           /* terminate now */
}

static void comment(void)
{
    int c;

    while ((c = yyinput()) != 0)
        if (c == '*')
        {
            while ((c = yyinput()) == '*')
                ;

            if (c == '/')
                return;

            if (c == 0)
                break;
        }
    yyerror("unterminated comment");
}

int column = 0;

void count()
{
	int i;

	for (i = 0; yytext[i] != '\0'; i++)
		if (yytext[i] == '\n')
			column = 0,line++;
		else if (yytext[i] == '\t')
			column += 8 - (column % 8);
		else
			column++;
	ECHO;
}

int hash_string_to_int(const string&s)
{
    int t=string_to_int[s];
    if (t==0)
    {
        t=string_to_int[s]=++total_string;
        for(;int_to_string.size()<t;int_to_string.push_back(""));
        int_to_string.push_back(s);
    }
    return t;
}

static int check_type(void)
{
	yylval=new node(yytext);
    hash_string_to_int(yytext);
    switch (sym_type(yytext))
    {
    case TYPEDEF_NAME:                /* previously defined */
        return TYPEDEF_NAME;
    case ENUMERATION_CONSTANT:        /* previously defined */
        return ENUMERATION_CONSTANT;
    default:                          /* includes undefined */
        return IDENTIFIER;
    }
}

int main(int argv,char* argc[])
{
    int_to_string.push_back("");
	if (argv==2)
	{
		FILE* in=fopen(argc[1],"r");
		yyin=in;
	}
	yyparse();
}


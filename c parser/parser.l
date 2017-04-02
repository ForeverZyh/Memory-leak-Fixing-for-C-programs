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
int sym_table[10000];
vector<node*> forest;
int cnt;
map<string,int> string_to_int;
int total_string=0;
vector<string> int_to_string;

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

//#define sym_type(identifier) IDENTIFIER /* with no symbol table, fake it */

static void comment(void);
static int check_type(void);
void count();
%}

%%
"/*"                                    { count();comment(); }
"//".*                                    { count();/* consume //-comment */ }

"auto"					{ count(); return(AUTO); }
"break"					{ count(); return(BREAK); }
"case"					{ count(); return(CASE); }
"char"					{ count(); return(CHAR); }
"const"					{ count(); return(CONST); }
"continue"				{ count(); return(CONTINUE); }
"default"				{ count(); return(DEFAULT); }
"do"					{ count(); return(DO); }
"double"				{ count(); return(DOUBLE); }
"else"					{ count(); return(ELSE); }
"enum"					{ count(); return(ENUM); }
"extern"				{ count(); return(EXTERN); }
"float"					{ count(); return(FLOAT); }
"for"					{ count(); return(FOR); }
"goto"					{ count(); return(GOTO); }
"if"					{ count(); return(IF); }
"inline"				{ count(); return(INLINE); }
"int"					{ count(); return(INT); }
"long"					{ count(); return(LONG); }
"register"				{ count(); return(REGISTER); }
"restrict"				{ count(); return(RESTRICT); }
"return"				{ count(); return(RETURN); }
"short"					{ count(); return(SHORT); }
"signed"				{ count(); return(SIGNED); }
"sizeof"				{ count(); return(SIZEOF); }
"static"				{ count(); return(STATIC); }
"struct"				{ count(); return(STRUCT); }
"switch"				{ count(); return(SWITCH); }
"typedef"				{ count(); return(TYPEDEF); }
"union"					{ count(); return(UNION); }
"unsigned"				{ count(); return(UNSIGNED); }
"void"					{ count(); return(VOID); }
"volatile"				{ count(); return(VOLATILE); }
"while"					{ count(); return(WHILE); }
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

{HP}{H}+{IS}?				{ count(); return I_CONSTANT; }
{NZ}{D}*{IS}?				{ count(); return I_CONSTANT; }
"0"{O}*{IS}?				{ count(); return I_CONSTANT; }
{CP}?"'"([^'\\\n]|{ES})+"'"		{ count(); return I_CONSTANT; }

{D}+{E}{FS}?				{ count(); return F_CONSTANT; }
{D}*"."{D}+{E}?{FS}?			{ count(); return F_CONSTANT; }
{D}+"."{E}?{FS}?			{ count(); return F_CONSTANT; }
{HP}{H}+{P}{FS}?			{ count(); return F_CONSTANT; }
{HP}{H}*"."{H}+{P}{FS}?			{ count(); return F_CONSTANT; }
{HP}{H}+"."{P}{FS}?			{ count(); return F_CONSTANT; }

({SP}?\"([^"\\\n]|{ES})*\"{WS}*)+	{ count(); return STRING_LITERAL; }

"..."					{ count(); return ELLIPSIS; }
">>="					{ count(); return RIGHT_ASSIGN; }
"<<="					{ count(); return LEFT_ASSIGN; }
"+="					{ count(); return ADD_ASSIGN; }
"-="					{ count(); return SUB_ASSIGN; }
"*="					{ count(); return MUL_ASSIGN; }
"/="					{ count(); return DIV_ASSIGN; }
"%="					{ count(); return MOD_ASSIGN; }
"&="					{ count(); return AND_ASSIGN; }
"^="					{ count(); return XOR_ASSIGN; }
"|="					{ count(); return OR_ASSIGN; }
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
("{"|"<%")				{ count(); return '{'; }
("}"|"%>")				{ count(); return '}'; }
","					{ count(); return ','; }
":"					{ count(); return ':'; }
"="					{ count(); return '='; }
"("					{ count(); return '('; }
")"					{ count(); return ')'; }
("["|"<:")				{ count(); return '['; }
("]"|":>")				{ count(); return ']'; }
"."					{ count(); return '.'; }
"&"					{ count(); return '&'; }
"!"					{ count(); return '!'; }
"~"					{ count(); return '~'; }
"-"					{ count(); return '-'; }
"+"					{ count(); return '+'; }
"*"					{ count(); return '*'; }
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

void dfs(node*r,int num)
{
    for(int i=0;i<num;i++) cout<<"  ";
    cout<<r->str<<endl;
    for(int i=0;i<(int)r->son.size();i++) dfs(r->son[i],num+1);
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
    //dfs(root,0);
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
			column = 0;
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


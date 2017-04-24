%{
#include "myheader.h"
#define YYSTYPE myYYSTYPE
extern char yytext[];
extern int column;
extern int sym_table[10000];
extern int cnt;
extern vector<node*> forest;
extern int yylex(void);
extern map<string,int> string_to_int;
extern int total_string;
extern vector<string> int_to_string;
extern "C"
{
	void yyerror(char *s);
}
void yyerror(char *s)
{
	fflush(stdout);
	printf("\n%*s\n%*s\n", column, "^", column, s);

}
%}


%token	IDENTIFIER I_CONSTANT F_CONSTANT STRING_LITERAL FUNC_NAME SIZEOF
%token	PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token	AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token	SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token	XOR_ASSIGN OR_ASSIGN
%token	TYPEDEF_NAME ENUMERATION_CONSTANT

%token	TYPEDEF EXTERN STATIC AUTO REGISTER INLINE
%token	CONST RESTRICT VOLATILE
%token	BOOL CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE VOID
%token	COMPLEX IMAGINARY 
%token	STRUCT UNION ENUM ELLIPSIS

%token	CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%token	ALIGNAS ALIGNOF ATOMIC GENERIC NORETURN STATIC_ASSERT THREAD_LOCAL

%start translation_unit
%%

primary_expression
	: IDENTIFIER
	| constant
	{
		$$=new node();
		forest.push_back($$);
		$$->str="primary_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| string
	{
		$$=new node();
		forest.push_back($$);
		$$->str="primary_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| '(' expression ')'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="primary_expression";
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	| generic_selection
	{
		$$=new node();
		forest.push_back($$);
		$$->str="primary_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	;

constant
	: I_CONSTANT		
	| F_CONSTANT
	| ENUMERATION_CONSTANT	
	;

enumeration_constant		
	: IDENTIFIER
	;

string
	: STRING_LITERAL
	| FUNC_NAME
	;

generic_selection
	: GENERIC '(' assignment_expression ',' generic_assoc_list ')'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="generic_selection";
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
		if ($5){
			$$->son.push_back($5);
			$5->fa=$$;}
	}
	;

generic_assoc_list
	: generic_association
	{
		$$=new node();
		forest.push_back($$);
		$$->str="generic_assoc_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| generic_assoc_list ',' generic_association
	{
		$$=new node();
		forest.push_back($$);
		$$->str="generic_assoc_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	;

generic_association
	: type_name ':' assignment_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="generic_association";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	| DEFAULT ':' assignment_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="generic_association";
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	;

postfix_expression
	: primary_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="postfix_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| postfix_expression '[' expression ']'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="postfix_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	| postfix_expression '(' ')'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="postfix_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| postfix_expression '(' argument_expression_list ')'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="postfix_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	| postfix_expression '.' IDENTIFIER
	{
		$$=new node();
		forest.push_back($$);
		$$->str="postfix_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| postfix_expression PTR_OP IDENTIFIER
	{
		$$=new node();
		forest.push_back($$);
		$$->str="postfix_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| postfix_expression INC_OP
	{
		$$=new node();
		forest.push_back($$);
		$$->str="postfix_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| postfix_expression DEC_OP
	{
		$$=new node();
		forest.push_back($$);
		$$->str="postfix_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| '(' type_name ')' '{' initializer_list '}'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="postfix_expression";
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
		if ($5){
			$$->son.push_back($5);
			$5->fa=$$;}
	}
	| '(' type_name ')' '{' initializer_list ',' '}'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="postfix_expression";
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
		if ($5){
			$$->son.push_back($5);
			$5->fa=$$;}
	}
	;

argument_expression_list
	: assignment_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="argument_expression_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| argument_expression_list ',' assignment_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="argument_expression_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	;

unary_expression
	: postfix_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="unary_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| INC_OP unary_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="unary_expression";
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	| DEC_OP unary_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="unary_expression";
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	| unary_operator cast_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="unary_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	| SIZEOF unary_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="unary_expression";
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	| SIZEOF '(' type_name ')'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="unary_expression";
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	| ALIGNOF '(' type_name ')'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="unary_expression";
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	;

unary_operator
	: '&'
	| '*'
	| '+'
	| '-'
	| '~'
	| '!'
	;

cast_expression
	: unary_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="cast_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| '(' type_name ')' cast_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="cast_expression";
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
		if ($4){
			$$->son.push_back($4);
			$4->fa=$$;}
	}
	;

multiplicative_expression
	: cast_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="multiplicative_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| multiplicative_expression '*' cast_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="multiplicative_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	| multiplicative_expression '/' cast_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="multiplicative_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	| multiplicative_expression '%' cast_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="multiplicative_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	;

additive_expression
	: multiplicative_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="additive_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| additive_expression '+' multiplicative_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="additive_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	| additive_expression '-' multiplicative_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="additive_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	;

shift_expression
	: additive_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="shift_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| shift_expression LEFT_OP additive_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="shift_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	| shift_expression RIGHT_OP additive_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="shift_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	;

relational_expression
	: shift_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="relational_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| relational_expression '<' shift_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="relational_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	| relational_expression '>' shift_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="relational_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	| relational_expression LE_OP shift_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="relational_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	| relational_expression GE_OP shift_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="relational_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	;

equality_expression
	: relational_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="equality_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| equality_expression EQ_OP relational_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="equality_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	| equality_expression NE_OP relational_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="equality_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	;

and_expression
	: equality_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="and_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| and_expression '&' equality_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="and_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	;

exclusive_or_expression
	: and_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="exclusive_or_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| exclusive_or_expression '^' and_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="exclusive_or_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	;

inclusive_or_expression
	: exclusive_or_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="inclusive_or_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| inclusive_or_expression '|' exclusive_or_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="inclusive_or_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	;

logical_and_expression
	: inclusive_or_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="logical_and_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| logical_and_expression AND_OP inclusive_or_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="logical_and_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	;

logical_or_expression
	: logical_and_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="logical_or_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| logical_or_expression OR_OP logical_and_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="logical_or_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	;

conditional_expression
	: logical_or_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="conditional_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| logical_or_expression '?' expression ':' conditional_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="conditional_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
		if ($5){
			$$->son.push_back($5);
			$5->fa=$$;}
	}
	;

assignment_expression
	: conditional_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="assignment_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| unary_expression assignment_operator assignment_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="assignment_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	;

assignment_operator
	: '='
	{
		$$=new node();
		forest.push_back($$);
		$$->str="=";
	}
	| MUL_ASSIGN
	| DIV_ASSIGN
	| MOD_ASSIGN
	| ADD_ASSIGN
	| SUB_ASSIGN
	| LEFT_ASSIGN
	| RIGHT_ASSIGN
	| AND_ASSIGN
	| XOR_ASSIGN
	| OR_ASSIGN
	;

expression
	: assignment_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| expression ',' assignment_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	;

constant_expression
	: conditional_expression	
	{
		$$=new node();
		forest.push_back($$);
		$$->str="constant_expression";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	;

declaration
	: declaration_specifiers ';'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="declaration";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| declaration_specifiers init_declarator_list ';'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="declaration";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	| static_assert_declaration
	{
		$$=new node();
		forest.push_back($$);
		$$->str="declaration";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	;

declaration_specifiers
	: storage_class_specifier declaration_specifiers
	{
		$$=new node();
		forest.push_back($$);
		$$->str="declaration_specifiers";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	| storage_class_specifier
	{
		$$=new node();
		forest.push_back($$);
		$$->str="declaration_specifiers";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| type_specifier declaration_specifiers
	{
		$$=new node();
		forest.push_back($$);
		$$->str="declaration_specifiers";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	| type_specifier
	{
		$$=new node();
		forest.push_back($$);
		$$->str="declaration_specifiers";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| type_qualifier declaration_specifiers
	{
		$$=new node();
		forest.push_back($$);
		$$->str="declaration_specifiers";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	| type_qualifier
	{
		$$=new node();
		forest.push_back($$);
		$$->str="declaration_specifiers";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| function_specifier declaration_specifiers
	{
		$$=new node();
		forest.push_back($$);
		$$->str="declaration_specifiers";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	| function_specifier
	{
		$$=new node();
		forest.push_back($$);
		$$->str="declaration_specifiers";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| alignment_specifier declaration_specifiers
	{
		$$=new node();
		forest.push_back($$);
		$$->str="declaration_specifiers";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	| alignment_specifier
	{
		$$=new node();
		forest.push_back($$);
		$$->str="declaration_specifiers";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	;

init_declarator_list
	: init_declarator
	{
		$$=new node();
		forest.push_back($$);
		$$->str="init_declarator_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| init_declarator_list ',' init_declarator
	{
		$$=new node();
		forest.push_back($$);
		$$->str="init_declarator_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	;

init_declarator
	: declarator '=' initializer
	{
		$$=new node();
		forest.push_back($$);
		$$->str="init_declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	| declarator
	{
		$$=new node();
		forest.push_back($$);
		$$->str="init_declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	;

storage_class_specifier
	: TYPEDEF	
	| EXTERN
	| STATIC
	| THREAD_LOCAL
	| AUTO
	| REGISTER
	;

type_specifier
	: VOID
	| CHAR
	| SHORT
	| INT
	| LONG
	| FLOAT
	| DOUBLE
	| SIGNED
	| UNSIGNED
	| BOOL
	| COMPLEX
	| IMAGINARY
	| atomic_type_specifier
	{
		$$=new node();
		forest.push_back($$);
		$$->str="type_specifier";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| struct_or_union_specifier
	{
		$$=new node();
		forest.push_back($$);
		$$->str="type_specifier";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| enum_specifier
	{
		$$=new node();
		forest.push_back($$);
		$$->str="type_specifier";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| TYPEDEF_NAME
	;

struct_or_union_specifier
	: struct_or_union '{' struct_declaration_list '}'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="struct_or_union_specifier";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	| struct_or_union IDENTIFIER '{' struct_declaration_list '}'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="struct_or_union_specifier";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($4){
			$$->son.push_back($4);
			$4->fa=$$;}
	}
	| struct_or_union IDENTIFIER
	{
		$$=new node();
		forest.push_back($$);
		$$->str="struct_or_union_specifier";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	;

struct_or_union
	: STRUCT
	| UNION
	;

struct_declaration_list
	: struct_declaration
	{
		$$=new node();
		forest.push_back($$);
		$$->str="struct_declaration_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| struct_declaration_list struct_declaration
	{
		$$=new node();
		forest.push_back($$);
		$$->str="struct_declaration_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	;

struct_declaration
	: specifier_qualifier_list ';'	
	{
		$$=new node();
		forest.push_back($$);
		$$->str="struct_declaration";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| specifier_qualifier_list struct_declarator_list ';'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="struct_declaration";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	| static_assert_declaration
	{
		$$=new node();
		forest.push_back($$);
		$$->str="struct_declaration";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list
	{
		$$=new node();
		forest.push_back($$);
		$$->str="specifier_qualifier_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	| type_specifier
	{
		$$=new node();
		forest.push_back($$);
		$$->str="specifier_qualifier_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| type_qualifier specifier_qualifier_list
	{
		$$=new node();
		forest.push_back($$);
		$$->str="specifier_qualifier_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	| type_qualifier
	{
		$$=new node();
		forest.push_back($$);
		$$->str="specifier_qualifier_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	;

struct_declarator_list
	: struct_declarator
	{
		$$=new node();
		forest.push_back($$);
		$$->str="struct_declarator_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| struct_declarator_list ',' struct_declarator
	{
		$$=new node();
		forest.push_back($$);
		$$->str="struct_declarator_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	;

struct_declarator
	: ':' constant_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="struct_declarator";
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	| declarator ':' constant_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="struct_declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	| declarator
	{
		$$=new node();
		forest.push_back($$);
		$$->str="struct_declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	;

enum_specifier
	: ENUM '{' enumerator_list '}'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="enum_specifier";
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	| ENUM '{' enumerator_list ',' '}'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="enum_specifier";
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	| ENUM IDENTIFIER '{' enumerator_list '}'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="enum_specifier";
		if ($4){
			$$->son.push_back($4);
			$4->fa=$$;}
	}
	| ENUM IDENTIFIER '{' enumerator_list ',' '}'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="enum_specifier";
		if ($4){
			$$->son.push_back($4);
			$4->fa=$$;}
	}
	| ENUM IDENTIFIER
	;

enumerator_list
	: enumerator
	{
		$$=new node();
		forest.push_back($$);
		$$->str="enumerator_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| enumerator_list ',' enumerator
	{
		$$=new node();
		forest.push_back($$);
		$$->str="enumerator_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	;

enumerator	
	: enumeration_constant '=' constant_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="enumerator	";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	| enumeration_constant
	{
		$$=new node();
		forest.push_back($$);
		$$->str="enumerator	";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	;

atomic_type_specifier
	: ATOMIC '(' type_name ')'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="atomic_type_specifier";
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	;

type_qualifier
	: CONST
	| RESTRICT
	| VOLATILE
	| ATOMIC
	;

function_specifier
	: INLINE
	| NORETURN
	;

alignment_specifier
	: ALIGNAS '(' type_name ')'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="alignment_specifier";
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	| ALIGNAS '(' constant_expression ')'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="alignment_specifier";
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	;

declarator
	: pointer direct_declarator
	{
		$$=new node();
		forest.push_back($$);
		$$->str="declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	| direct_declarator
	{
		$$=new node();
		forest.push_back($$);
		$$->str="declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	;

direct_declarator
	: IDENTIFIER
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| '(' declarator ')'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_declarator";
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	| direct_declarator '[' ']'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| direct_declarator '[' '*' ']'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| direct_declarator '[' STATIC type_qualifier_list assignment_expression ']'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($4){
			$$->son.push_back($4);
			$4->fa=$$;}
		if ($5){
			$$->son.push_back($5);
			$5->fa=$$;}
	}
	| direct_declarator '[' STATIC assignment_expression ']'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($4){
			$$->son.push_back($4);
			$4->fa=$$;}
	}
	| direct_declarator '[' type_qualifier_list '*' ']'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	| direct_declarator '[' type_qualifier_list STATIC assignment_expression ']'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
		if ($5){
			$$->son.push_back($5);
			$5->fa=$$;}
	}
	| direct_declarator '[' type_qualifier_list assignment_expression ']'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
		if ($4){
			$$->son.push_back($4);
			$4->fa=$$;}
	}
	| direct_declarator '[' type_qualifier_list ']'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	| direct_declarator '[' assignment_expression ']'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	| direct_declarator '(' parameter_type_list ')'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	| direct_declarator '(' ')'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| direct_declarator '(' identifier_list ')'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	;

pointer
	: '*' type_qualifier_list pointer
	{
		$$=new node();
		forest.push_back($$);
		$$->str="pointer";
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	| '*' type_qualifier_list
	{
		$$=new node();
		forest.push_back($$);
		$$->str="pointer";
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	| '*' pointer
	{
		$$=new node();
		forest.push_back($$);
		$$->str="pointer";
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	| '*'
	;

type_qualifier_list
	: type_qualifier
	{
		$$=new node();
		forest.push_back($$);
		$$->str="type_qualifier_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| type_qualifier_list type_qualifier
	{
		$$=new node();
		forest.push_back($$);
		$$->str="type_qualifier_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	;


parameter_type_list
	: parameter_list ',' ELLIPSIS
	{
		$$=new node();
		forest.push_back($$);
		$$->str="parameter_type_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| parameter_list
	{
		$$=new node();
		forest.push_back($$);
		$$->str="parameter_type_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	;

parameter_list
	: parameter_declaration
	{
		$$=new node();
		forest.push_back($$);
		$$->str="parameter_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| parameter_list ',' parameter_declaration
	{
		$$=new node();
		forest.push_back($$);
		$$->str="parameter_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	;

parameter_declaration
	: declaration_specifiers declarator
	{
		$$=new node();
		forest.push_back($$);
		$$->str="parameter_declaration";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	| declaration_specifiers abstract_declarator
	{
		$$=new node();
		forest.push_back($$);
		$$->str="parameter_declaration";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	| declaration_specifiers
	{
		$$=new node();
		forest.push_back($$);
		$$->str="parameter_declaration";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	;

identifier_list
	: IDENTIFIER
	| identifier_list ',' IDENTIFIER
	{
		$$=new node();
		forest.push_back($$);
		$$->str="identifier_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	;

type_name
	: specifier_qualifier_list abstract_declarator
	{
		$$=new node();
		forest.push_back($$);
		$$->str="type_name";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	| specifier_qualifier_list
	{
		$$=new node();
		forest.push_back($$);
		$$->str="type_name";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	;

abstract_declarator
	: pointer direct_abstract_declarator
	{
		$$=new node();
		forest.push_back($$);
		$$->str="abstract_declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	| pointer
	{
		$$=new node();
		forest.push_back($$);
		$$->str="abstract_declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| direct_abstract_declarator
	{
		$$=new node();
		forest.push_back($$);
		$$->str="abstract_declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	;

direct_abstract_declarator
	: '(' abstract_declarator ')'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_abstract_declarator";
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	| '[' ']'
	| '[' '*' ']'
	| '[' STATIC type_qualifier_list assignment_expression ']'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_abstract_declarator";
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
		if ($4){
			$$->son.push_back($4);
			$4->fa=$$;}
	}
	| '[' STATIC assignment_expression ']'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_abstract_declarator";
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	| '[' type_qualifier_list STATIC assignment_expression ']'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_abstract_declarator";
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
		if ($4){
			$$->son.push_back($4);
			$4->fa=$$;}
	}
	| '[' type_qualifier_list assignment_expression ']'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_abstract_declarator";
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	| '[' type_qualifier_list ']'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_abstract_declarator";
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	| '[' assignment_expression ']'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_abstract_declarator";
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	| direct_abstract_declarator '[' ']'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_abstract_declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| direct_abstract_declarator '[' '*' ']'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_abstract_declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| direct_abstract_declarator '[' STATIC type_qualifier_list assignment_expression ']'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_abstract_declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($4){
			$$->son.push_back($4);
			$4->fa=$$;}
		if ($5){
			$$->son.push_back($5);
			$5->fa=$$;}
	}
	| direct_abstract_declarator '[' STATIC assignment_expression ']'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_abstract_declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($4){
			$$->son.push_back($4);
			$4->fa=$$;}
	}
	| direct_abstract_declarator '[' type_qualifier_list assignment_expression ']'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_abstract_declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
		if ($4){
			$$->son.push_back($4);
			$4->fa=$$;}
	}
	| direct_abstract_declarator '[' type_qualifier_list STATIC assignment_expression ']'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_abstract_declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
		if ($5){
			$$->son.push_back($5);
			$5->fa=$$;}
	}
	| direct_abstract_declarator '[' type_qualifier_list ']'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_abstract_declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	| direct_abstract_declarator '[' assignment_expression ']'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_abstract_declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	| '(' ')'
	| '(' parameter_type_list ')'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_abstract_declarator";
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	| direct_abstract_declarator '(' ')'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_abstract_declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| direct_abstract_declarator '(' parameter_type_list ')'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="direct_abstract_declarator";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	;

initializer
	: '{' initializer_list '}'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="initializer";
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	| '{' initializer_list ',' '}'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="initializer";
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	| assignment_expression
	{
		$$=new node();
		forest.push_back($$);
		$$->str="initializer1";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	;

initializer_list
	: designation initializer
	{
		$$=new node();
		forest.push_back($$);
		$$->str="initializer_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	| initializer
	{
		$$=new node();
		forest.push_back($$);
		$$->str="initializer_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| initializer_list ',' designation initializer
	{
		$$=new node();
		forest.push_back($$);
		$$->str="initializer_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
		if ($4){
			$$->son.push_back($4);
			$4->fa=$$;}
	}
	| initializer_list ',' initializer
	{
		$$=new node();
		forest.push_back($$);
		$$->str="initializer_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	;

designation
	: designator_list '='
	{
		$$=new node();
		forest.push_back($$);
		$$->str="designation";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	;

designator_list
	: designator
	{
		$$=new node();
		forest.push_back($$);
		$$->str="designator_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| designator_list designator
	{
		$$=new node();
		forest.push_back($$);
		$$->str="designator_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	;

designator
	: '[' constant_expression ']'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="designator";
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	| '.' IDENTIFIER
	;

static_assert_declaration
	: STATIC_ASSERT '(' constant_expression ',' STRING_LITERAL ')' ';'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="static_assert_declaration";
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	;

statement
	: labeled_statement
	{
		$$=new node();
		forest.push_back($$);
		$$->str="statement";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| compound_statement
	{
		$$=new node();
		forest.push_back($$);
		$$->str="statement";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| expression_statement
	{
		$$=new node();
		forest.push_back($$);
		$$->str="statement";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| selection_statement
	{
		$$=new node();
		forest.push_back($$);
		$$->str="statement";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| iteration_statement
	{
		$$=new node();
		forest.push_back($$);
		$$->str="statement";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| jump_statement
	{
		$$=new node();
		forest.push_back($$);
		$$->str="statement";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	;

labeled_statement
	: IDENTIFIER ':' statement
	{
		$$=new node();
		forest.push_back($$);
		$$->str="labeled_statement";
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	| CASE constant_expression ':' statement
	{
		$$=new node();
		forest.push_back($$);
		$$->str="labeled_statement";
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
		if ($4){
			$$->son.push_back($4);
			$4->fa=$$;}
	}
	| DEFAULT ':' statement
	{
		$$=new node();
		forest.push_back($$);
		$$->str="labeled_statement";
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	;

compound_statement
	: '{' '}'
	| '{' block_item_list '}'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="compound_statement";
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	;

block_item_list
	: block_item
	{
		$$=new node();
		forest.push_back($$);
		$$->str="block_item_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| block_item_list block_item
	{
		$$=new node();
		forest.push_back($$);
		$$->str="block_item_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	;

block_item
	: declaration
	{
		$$=new node();
		forest.push_back($$);
		$$->str="block_item";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| statement
	{
		$$=new node();
		forest.push_back($$);
		$$->str="block_item";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	;

expression_statement
	: ';'
	| expression ';'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="expression_statement";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	;

selection_statement
	: IF '(' expression ')' statement ELSE statement
	{
		$$=new node();
		forest.push_back($$);
		$$->str="IF ELSE";
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
		if ($5){
			$$->son.push_back($5);
			$5->fa=$$;}
		if ($7){
			$$->son.push_back($7);
			$7->fa=$$;}
	}
	| IF '(' expression ')' statement
	{
		$$=new node();
		forest.push_back($$);
		$$->str="IF";
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
		if ($5){
			$$->son.push_back($5);
			$5->fa=$$;}
	}
	| SWITCH '(' expression ')' statement
	{
		$$=new node();
		forest.push_back($$);
		$$->str="selection_statement";
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
		if ($5){
			$$->son.push_back($5);
			$5->fa=$$;}
	}
	;

iteration_statement
	: WHILE '(' expression ')' statement
	{
		$$=new node();
		forest.push_back($$);
		$$->str="WHILE";
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
		if ($5){
			$$->son.push_back($5);
			$5->fa=$$;}
	}
	| DO statement WHILE '(' expression ')' ';'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="DO WHILE";
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
		if ($5){
			$$->son.push_back($5);
			$5->fa=$$;}
	}
	| FOR '(' expression_statement expression_statement ')' statement
	{
		$$=new node();
		forest.push_back($$);
		$$->str="FOR(x;x;)";
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
		if ($4){
			$$->son.push_back($4);
			$4->fa=$$;}
		if ($6){
			$$->son.push_back($6);
			$6->fa=$$;}
	}
	| FOR '(' expression_statement expression_statement expression ')' statement
	{
		$$=new node();
		forest.push_back($$);
		$$->str="FOR(x;x;x)";
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
		if ($4){
			$$->son.push_back($4);
			$4->fa=$$;}
		if ($5){
			$$->son.push_back($5);
			$5->fa=$$;}
		if ($7){
			$$->son.push_back($7);
			$7->fa=$$;}
	}
	| FOR '(' declaration expression_statement ')' statement
	{
		$$=new node();
		forest.push_back($$);
		$$->str="FOR(d;x;)";
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
		if ($4){
			$$->son.push_back($4);
			$4->fa=$$;}
		if ($6){
			$$->son.push_back($6);
			$6->fa=$$;}
	}
	| FOR '(' declaration expression_statement expression ')' statement
	{
		$$=new node();
		forest.push_back($$);
		$$->str="FOR(d;x;x)";
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
		if ($4){
			$$->son.push_back($4);
			$4->fa=$$;}
		if ($5){
			$$->son.push_back($5);
			$5->fa=$$;}
		if ($7){
			$$->son.push_back($7);
			$7->fa=$$;}
	}
	;

jump_statement
	: GOTO IDENTIFIER ';'
	| CONTINUE ';'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="CONTINUE";
	}
	| BREAK ';'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="BREAK";
	}
	| RETURN ';'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="RETURN";
	}
	| RETURN expression ';'
	{
		$$=new node();
		forest.push_back($$);
		$$->str="RETURN";
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	;

translation_unit
	: external_declaration
	{
		$$=new node();
		forest.push_back($$);
		$$->str="translation_unit";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| translation_unit external_declaration
	{
		$$=new node();
		forest.push_back($$);
		$$->str="translation_unit";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	;

external_declaration
	: function_definition
	{
		$$=new node();
		forest.push_back($$);
		$$->str="external_declaration";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| declaration
	{
		$$=new node();
		forest.push_back($$);
		$$->str="external_declaration";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	;

function_definition
	: declaration_specifiers declarator declaration_list compound_statement
	{
		$$=new node();
		forest.push_back($$);
		$$->str="function_definition";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
		if ($4){
			$$->son.push_back($4);
			$4->fa=$$;}
	}
	| declaration_specifiers declarator compound_statement
	{
		$$=new node();
		forest.push_back($$);
		$$->str="function_definition";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
		if ($3){
			$$->son.push_back($3);
			$3->fa=$$;}
	}
	;

declaration_list
	: declaration
	{
		$$=new node();
		forest.push_back($$);
		$$->str="declaration_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
	}
	| declaration_list declaration
	{
		$$=new node();
		forest.push_back($$);
		$$->str="declaration_list";
		if ($1){
			$$->son.push_back($1);
			$1->fa=$$;}
		if ($2){
			$$->son.push_back($2);
			$2->fa=$$;}
	}
	;


%%

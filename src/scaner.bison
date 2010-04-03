%{
	
#include <stdio.h>

#define YY_DECL int yylex(yyscan_t yyscanner,YYSTYPE *yylval,void * private_data)
#include "scaner.tab.h"
//#include "scaner.lex.h"

typedef void * yyscan_t;

int yylex(YYSTYPE * ,yyscan_t,void * private_data);
void yyerror(yyscan_t,void *private_data,char *);

#define YYLEX_PARAM	scanner,&yylval,private_data

%}

%union {
	int val;
	char	op;
}

%lex-param{YYSTYPE *yylval,yyscan_t scanner}
/* parse param */
%parse-param {yyscan_t scanner}
%parse-param {void *private_data}

%token <val> NUMBER 
%token NAME
%token <op> OP

%left	'+' '-'
%left '*' '/'
%nonassoc UMINUS

%type <val>	expression
%%



statement:NAME '=' expression
		| expression { printf( " = %d\n"  , $1 ) ;   }
			;

expression:			
			  expression '+' expression   { $$ = $1 + $3 ; }
			 | expression '-' expression   { $$ = $1 - $3 ; }
			 | expression '*' expression   { $$ = $1 * $3 ; }
			 | expression '/' expression   { $$ = $1 / $3 ; }
			| '-' expression  {  $$ = -$2;   }			
			| '(' expression  ')'  {  $$ = $2;   }						
			| NUMBER	{  $$ = $1 ;  }
			;

%%

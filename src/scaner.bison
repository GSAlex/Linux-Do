%{
	
#include <stdio.h>

#define YY_DECL int yylex(yyscan_t yyscanner,YYSTYPE *yylval,void * private_data)
#include "scaner.tab.h"

typedef void * yyscan_t;

int yylex(YYSTYPE * ,yyscan_t,void * private_data);
void yyerror(yyscan_t,void *private_data,char *);

#define YYLEX_PARAM	scanner,&yylval,private_data

%}

%union {
	char * tok;
}

%lex-param{YYSTYPE *yylval,yyscan_t scanner}
/* parse param */
%parse-param {yyscan_t scanner}
%parse-param {void *private_data}

%token <tok> KEYWORD

%token KEYWORD_TYPE 
%token <tok> NAME
%token <tok> sentence
%token <tok> STRUCT
%token <tok> TYPEDEF
%token <tok> INCLUDE
%token <tok> TYPE

%left	'+' '-'
%left '*' '/'
%nonassoc UMINUS

%%

c_statements : c_statement |
				c_statements c_statement ;

c_statement: defination | include ;

type:	KEYWORD 
	| TYPE ;
	
	
	
	
var_define : type NAME ';' ;

defination : function | function_body | struct |typedef | var_define ;


function:	type NAME '(' sentence  ')' ';' ;

function_body : function '{' sentence '}' ;

	 
var_define_list :  var_define 
				|  var_define_list var_define ;

struct : STRUCT '{' var_define_list '}' ';'  
		| STRUCT '{' var_define_list '}' NAME ';' ;
		
typedef : TYPEDEF type NAME ';' 
		;

include : INCLUDE '<' sentence '>' ;


%%

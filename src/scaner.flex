%{

#include<stdlib.h>
#include "scaner.tab.h"   
#define YY_DECL int yylex(yyscan_t yyscanner,YYSTYPE *yylval,void * private_data)

%}

%option header-file="scaner.lex.h"
%option reentrant

%x	COMMENT

%%

"/*"	BEGIN COMMENT;

<COMMENT>"*/"	BEGIN INITIAL;
<COMMENT>.	|
<COMMENT>\n	;

[\n\t ]+	;


[0-9]+		{  yylval->val = atoi(yytext); return NUMBER; }


;	return 0;

. return yytext[0];


%%

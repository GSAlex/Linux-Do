%{

#include<stdlib.h>
#include "scaner.tab.h"   
#define YY_DECL int yylex(yyscan_t yyscanner,YYSTYPE *yylval,void * private_data)

#define ECHO {;}

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

int	|
char	|
long	|
signed	|
unsigned {  yylval->tok = yytext; return KEYWORD;  }

typedef { yylval->tok = yytext ;  return TYPEDEF;  }

struct { yylval->tok = yytext ;  return STRUCT;  }


"#include"   {return INCLUDE;}

. return yytext[0];

%%

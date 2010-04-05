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

%token int_const char_const float_const id string enumeration_const
%token AUTO REGISTER STATIC EXTERN 
%token TYPEDEF VOID 
%token CHAR SHORT INT LONG FLOAT DOUBLE SIGNED UNSIGNED
%token CONST VOLATILE STRUCT UNION ENUM

%token OP_LESSEQU OP_MOREEQU OP_INC OP_DEC OP_PTR OP_EQU OP_NOTEQU  OP_SHIFT_LEFT OP_SHIFT_RIGHT
%token OP_EQU_MUL OP_EQU_DIV OP_EQU_REMAIN OP_EQU_ADD OP_EQU_SUB OP_EQU_SHIFT_LEFT OP_EQU_SHIFT_RIGHT OP_EQU_AND OP_EQU_XOR OP_EQU_OR OP_OR OP_AND
%token DOT_DOT_DOT SIZEOF

%left	'+' '-'
%left '*' '/'
%nonassoc UMINUS

%%

translation_unit	: external_decl
			| translation_unit external_decl
			;
external_decl		: function_definition
			| decl
			;
function_definition	: decl_specs declarator decl_list compound_stat
			|		declarator decl_list compound_stat
			| decl_specs declarator		compound_stat
			|		declarator 	compound_stat
			;
decl			: decl_specs init_declarator_list ';'
			| decl_specs			';'
			;
decl_list		: decl
			| decl_list decl
			;
decl_specs		: storage_class_spec decl_specs
			| storage_class_spec
			| type_spec decl_specs
			| type_spec
			| type_qualifier decl_specs
			| type_qualifier
			;
storage_class_spec	: AUTO | REGISTER | STATIC | EXTERN | TYPEDEF
			;
type_spec		: VOID | CHAR | SHORT | INT | LONG | FLOAT
			| DOUBLE | SIGNED | UNSIGNED
			| struct_or_union_spec
			| enum_spec
			| typedef_name
			;
type_qualifier		: CONST | VOLATILE
			;
struct_or_union_spec	: struct_or_union id '{' struct_decl_list '}'
			| struct_or_union	'{' struct_decl_list '}'
			| struct_or_union id
			;
struct_or_union		: STRUCT | UNION
			;
struct_decl_list	: struct_decl
			| struct_decl_list struct_decl
			;
init_declarator_list	: init_declarator
			| init_declarator_list ',' init_declarator
			;
init_declarator		: declarator
			| declarator '=' initializer
			;
struct_decl		: spec_qualifier_list struct_declarator_list ';'
			;
spec_qualifier_list	: type_spec spec_qualifier_list
			| type_spec
			| type_qualifier spec_qualifier_list
			| type_qualifier
			;
struct_declarator_list	: struct_declarator
			| struct_declarator_list ',' struct_declarator
			;
struct_declarator	: declarator
			| declarator ':' const_exp
			|		':' const_exp
			;
enum_spec		: ENUM id '{' enumerator_list '}'
			| ENUM	'{' enumerator_list '}'
			| ENUM id
			;
enumerator_list		: enumerator
			| enumerator_list ',' enumerator
			;
enumerator		: id
			| id '=' const_exp
			;
declarator		: pointer direct_declarator
			|	direct_declarator
			;
direct_declarator	: id
			| '(' declarator ')'
			| direct_declarator '[' const_exp ']'
			| direct_declarator '['		']'
			| direct_declarator '(' param_type_list ')'
			| direct_declarator '(' id_list ')'
			| direct_declarator '('		')'
			;
pointer			: '*' type_qualifier_list
			| '*'
			| '*' type_qualifier_list pointer
			| '*'			pointer
			;
type_qualifier_list	: type_qualifier
			| type_qualifier_list type_qualifier
			;
param_type_list		: param_list
			| param_list ',' DOT_DOT_DOT
			;
param_list		: param_decl
			| param_list ',' param_decl
			;
param_decl		: decl_specs declarator
			| decl_specs abstract_declarator
			| decl_specs
			;
id_list			: id
			| id_list ',' id
			;
initializer		: assignment_exp
			| '{' initializer_list '}'
			| '{' initializer_list ',' '}'
			;
initializer_list	: initializer
			| initializer_list ',' initializer
			;
type_name		: spec_qualifier_list abstract_declarator
			| spec_qualifier_list
			;
abstract_declarator	: pointer
			| pointer direct_abstract_declarator
			|	direct_abstract_declarator
			;
direct_abstract_declarator: '(' abstract_declarator ')'
			| direct_abstract_declarator '[' const_exp ']'
			|				'[' const_exp ']'
			| direct_abstract_declarator '['	']'
			|				'['	']'
			| direct_abstract_declarator '(' param_type_list ')'
			|				'(' param_type_list ')'
			| direct_abstract_declarator '('		')'
			|				'('		')'
			;
typedef_name		: id
			;
stat			: labeled_stat
			| exp_stat
			| compound_stat
			| selection_stat
			| iteration_stat
			| jump_stat
			;
labeled_stat		: id ':' stat
			| 'case' const_exp ':' stat
			| 'default' ':' stat
			;
exp_stat		: exp ';'
			|	';'
			;
compound_stat		: '{' decl_list stat_list '}'
			| '{'		stat_list '}'
			| '{' decl_list		'}'
			| '{'			'}'
			;
stat_list		: stat
			| stat_list stat
			;
selection_stat		: 'if' '(' exp ')' stat
			| 'if' '(' exp ')' stat 'else' stat
			| 'switch' '(' exp ')' stat
			;
iteration_stat		: 'while' '(' exp ')' stat
			| 'do' stat 'while' '(' exp ')' ';'
			| 'for' '(' exp ';' exp ';' exp ')' stat
			| 'for' '(' exp ';' exp ';'	')' stat
			| 'for' '(' exp ';'	';' exp ')' stat
			| 'for' '(' exp ';'	';'	')' stat
			| 'for' '('	';' exp ';' exp ')' stat
			| 'for' '('	';' exp ';'	')' stat
			| 'for' '('	';'	';' exp ')' stat
			| 'for' '('	';'	';'	')' stat
			;
jump_stat		: 'goto' id ';'
			| 'continue' ';'
			| 'break' ';'
			| 'return' exp ';'
			| 'return'	';'
			;
exp			: assignment_exp
			| exp ',' assignment_exp
			;
assignment_exp		: conditional_exp
			| unary_exp assignment_operator assignment_exp
			;
assignment_operator	: '=' | OP_EQU_MUL | OP_EQU_DIV | OP_EQU_REMAIN | OP_EQU_ADD | OP_EQU_SUB | OP_EQU_SHIFT_LEFT
			| OP_EQU_SHIFT_RIGHT | OP_EQU_AND | OP_EQU_XOR | OP_EQU_OR
			;
conditional_exp		: logical_or_exp
			| logical_or_exp '?' exp ':' conditional_exp
			;
const_exp		: conditional_exp
			;
logical_or_exp		: logical_and_exp
			| logical_or_exp OP_OR logical_and_exp
			;
logical_and_exp		: inclusive_or_exp
			| logical_and_exp OP_AND inclusive_or_exp
			;
inclusive_or_exp	: exclusive_or_exp
			| inclusive_or_exp '|' exclusive_or_exp
			;
exclusive_or_exp	: and_exp
			| exclusive_or_exp '^' and_exp
			;
and_exp			: equality_exp
			| and_exp '&' equality_exp
			;
equality_exp		: relational_exp
			| equality_exp OP_EQU relational_exp
			| equality_exp OP_NOTEQU relational_exp
			;
relational_exp		: shift_expression
			| relational_exp '<' shift_expression
			| relational_exp '>' shift_expression
			| relational_exp OP_LESSEQU shift_expression
			| relational_exp OP_MOREEQU shift_expression
			;
shift_expression	: additive_exp
			| shift_expression OP_SHIFT_LEFT additive_exp
			| shift_expression OP_SHIFT_RIGHT additive_exp
			;
additive_exp		: mult_exp
			| additive_exp '+' mult_exp
			| additive_exp '-' mult_exp
			;
mult_exp		: cast_exp
			| mult_exp '*' cast_exp
			| mult_exp '/' cast_exp
			| mult_exp '%' cast_exp
			;
cast_exp		: unary_exp
			| '(' type_name ')' cast_exp
			;
unary_exp		: postfix_exp
			| OP_INC unary_exp
			| OP_DEC unary_exp
			| unary_operator cast_exp
			| SIZEOF unary_exp
			| SIZEOF '(' type_name ')'
			;
unary_operator		: '&' | '*' | '+' | '-' | '~' | '!'
			;
postfix_exp		: primary_exp
			| postfix_exp '[' exp ']'
			| postfix_exp '(' argument_exp_list ')'
			| postfix_exp '('			')'
			| postfix_exp '.' id
			| postfix_exp OP_PTR id
			| postfix_exp OP_INC
			| postfix_exp OP_DEC
			;
primary_exp		: id
			| const
			| string
			| '(' exp ')'
			;
argument_exp_list	: assignment_exp
			| argument_exp_list ',' assignment_exp
			;
const			: int_const
			| char_const
			| float_const
			| enumeration_const
			;


%%

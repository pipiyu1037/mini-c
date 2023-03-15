%option  yylineno
%{
#include "string.h"

int ErrorCharNum=0;
int yycolumn=1;

#define YY_USER_ACTION    \
	yylloc.first_line=yylloc.last_line=yylineno; \
	yylloc.first_column=yycolumn;\
	yylloc.last_column=yycolumn+yyleng-1;\
	yycolumn+=yyleng;

typedef struct {
	int type_int;
	int type_float;
	char type_char;
	char type_id[32];
} YYLVAL;
#define YYSTYPE YYLVAL
#include "parser.tab.hpp"

%}

id   [A-Za-z][A-Za-z0-9]*
intconst    0|([1-9][0-9]*)
floatconst    [0-9]*\.?[0-9]?([eE][-+]?[0-9]+)?
charconst	'[A-Za-z]'
%%
"int"   	{strcpy(yylval.type_id,  yytext);return TYPE;}
"char"   	{strcpy(yylval.type_id,  yytext);return TYPE;}
"float"     {strcpy(yylval.type_id,  yytext);return TYPE;}
"void"      {strcpy(yylval.type_id,  yytext);return TYPE;}

"return"    {return RETURN;}
"if"       	{return IF;}
"else"      {return ELSE;}
"while"     {return WHILE;}
"for"       {return FOR;}
"break"		{return BREAK;}
"continue"	{return CONTINUE;}

{id}	{strcpy(yylval.type_id,yytext); return ID;}
";"	    {return SEMI;}
","	    {return COMMA;}
">="	{strcpy(yylval.type_id, yytext);return GE;}
">"	    {strcpy(yylval.type_id, yytext);return GT;}
"<="	{strcpy(yylval.type_id, yytext);return LE;}
"<"	    {strcpy(yylval.type_id, yytext);return LT;}
"!="	{strcpy(yylval.type_id, yytext);return NE;}
"=="	{strcpy(yylval.type_id, yytext);return EQ;}
"="	    {return ASSIGN;}
"++"	{return DPLUS;}
"+"	    {return PLUS;}
"-"	    {return MINUS;}
"--"	{return DMINUS;}
"*"	    {return STAR;}
"/"	    {return DIV;}
"&&"    {return AND;}
"||"    {return OR;}
"!"	    {return NOT;}
"("	    {return LP;}
")"	    {return RP;}
"{"	    {return LC;}
"}"	    {return RC;}
"["		{return LB;}
"]"		{return RB;}


{intconst}      { yylval.type_int=atoi(yytext); return INT;}
{floatconst}    { yylval.type_float=atof(yytext);     return FLOAT;}
{charconst}		{ yylval.type_char=yytext[1]; return CHAR;}
[ \r\t]     	{}

[\n]        	{yycolumn=1;}
.	{ printf("在第 %d 行出现不可识别的符号 \'%s\' \n",yylineno,yytext); ErrorCharNum++;}

%%

int yywrap()
{
return 1;
}

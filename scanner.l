%{
#include"node.h"
#include "syntax.tab.h"

extern unsigned lexError;
int lines = 0;
int yycolumn = 1;

#define YY_USER_ACTION \
    yylloc.first_line=yylloc.last_line=yylineno; \
    yylloc.first_column=yycolumn; \
    yylloc.last_column=yycolumn+yyleng-1; \
    yycolumn+=yyleng;
%}

%option yylineno
FUNCTION "function"
MAIN "main"
ws [ \t]
digit [0-9]
letter [_a-zA-Z]
ld[0-9_a-zA-Z]
nonletter [^_a-zA-Z]
IF if
ELSE else
FOR for
WHILE while
DO do
INT int 
WRITE write
READ read
SWITCH switch
CASE case
BREAK break
DEFAULT defaule
PLUS "+"
MINUS "-"
STAR "*"
DIV "/"
LP "("
RP ")"
SEMI ";"
COMMA ","
BIG ">"
LET "<"
LC "{"
RC "}"
NOT "!"
AND "&&"
OR "||"
NOTE "!="
EQ "=="
GE ">="
LE "<="
ASSIGNOP "="
ID {letter}{ld}*
NUM 0|[1-9]+[0-9]*
COMMENT_FIRST "/*"
COMMENT_LAST "*/"
%start COMMENT

RELOP BIG LET NOTE EQ GE LE ASSIGNOP AND OR 
%%
{ws}+ {;}
\n|\r { yycolumn = 1; }

{COMMENT_FIRST}         {
                    // 进入注释状态
                    BEGIN COMMENT; 
                return COMMENT_FIRST;}

<COMMENT>\*+[^/]* { /* 处理注释中的星号，避免提前结束 */ }

<COMMENT>[^*]+  { /* 忽略匹配的字符，继续在注释内部*/ }


<COMMENT>{COMMENT_LAST} { 
                    // 退出注释状态
                    BEGIN INITIAL; 
                return COMMENT_LAST;}
{FUNCTION} {yylval.node = newTokenNode(yylineno,TOKEN_OTHER,"FUNCTION",yytext);return FUNCTION;}
{MAIN} {yylval.node = newTokenNode(yylineno,TOKEN_OTHER,"MAIN",yytext);return MAIN;}
{IF} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "IF", yytext); return IF; }
{ELSE} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "ELSE", yytext); return ELSE; }
{WHILE} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "WHILE", yytext); return WHILE; }
{FOR} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "FOR", yytext); return FOR; }
{DO} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "DO", yytext); return DO; }
{INT} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "INT", yytext); return INT; }
{WRITE} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "WRITE", yytext); return WRITE; }
{READ} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "READ", yytext); return READ; }
{SWITCH} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "SWITCH", yytext); return SWITCH; }
{CASE} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "CASE", yytext); return CASE; }
{BREAK} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "BREAK", yytext); return BREAK; }
{DEFAULT} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "DEFAULT", yytext); return DEFAULT; }

{PLUS} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "PLUS", yytext); return PLUS; }
{MINUS} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "MINUS", yytext); return MINUS; }
{STAR} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "STAR", yytext); return STAR; }
{DIV} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "DIV", yytext); return DIV; }
{LP} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "LP", yytext); return LP; }
{RP} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "RP", yytext); return RP; }
{SEMI} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "SEMI", yytext); return SEMI; }
{COMMA} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "COMMA", yytext); return COMMA; }
{BIG} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "BIG", yytext); return BIG; }
{LET} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "LET", yytext); return LET; }
{LC} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "LC", yytext); return LC; }
{RC} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "RC", yytext); return RC; }
{NOT} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "NOT", yytext); return NOT; }
{AND} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "AND", yytext); return AND; }
{OR} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "OR", yytext); return OR; }

{NOTE} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "NOTE", yytext); return NOTE; }
{EQ} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "EQ", yytext); return EQ; }
{GE} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "GE", yytext); return GE; }
{LE} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "LE", yytext); return LE; }
{ASSIGNOP} { yylval.node = newTokenNode(yylineno, TOKEN_OTHER, "ASSIGNOP", yytext); return ASSIGNOP; }


{ID} { yylval.node = newTokenNode(yylineno, TOKEN_ID, "ID", yytext); return ID;}
{NUM} { yylval.node = newTokenNode(yylineno, TOKEN_NUM, "NUM", yytext); return NUM;}

{digit}+{ID} {lexError = TRUE; printf("Error type A at Line %d: Illegal ID \"%s\".\n", yylineno, yytext); }
. { lexError = TRUE; printf("Error type A at Line %d: Mysterious character \'%s\'.\n", yylineno, yytext); }
%%

// int main(int argc, char** argv) {
// if (argc > 1) {
// if (!(yyin = fopen(argv[1], "r"))) {
// perror(argv[1]);
// return 1;
// }
// }
// yylex();
// printf("%d\n",lines);
// return 0;
// }




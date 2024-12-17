%{
#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include"token.h"

int cur_line_num = 1;
void init_scanner();
void lex_error(char* msg,int line);
%}
%start COMMENT
DIGIT               [0-9]
LETTER              [a-zA-Z]
ID                  {LETTER}({LETTER}|{DIGIT})*
NUM                 {DIGIT}+
SINGLEWORD          [\+\-\*\/\=\(\)\{\}\;\,\!\&\:\<\>\ ]
DOUBLEWORD          (\>=|\<=|\!=|\==|\&\&|\|\|)
RESERVED_WORD       (if|else|for|while|do|int|write|read|switch|case|break|default)
COMMENT_FIRST      "/*"
COMMENT_LAST       "*/"
%%


\n                   { cur_line_num++; }

[ \t\r]+             { /* ignore whitespace */  }

{COMMENT_FIRST}         {
                    // 进入注释状态
                    BEGIN COMMENT; 
                return T_comment_first;}

<COMMENT>\*+[^/]* { /* 处理注释中的星号，避免提前结束 */ }

<COMMENT>[^*]+  { /* 忽略匹配的字符，继续在注释内部*/ }


<COMMENT>{COMMENT_LAST} { 
                    // 退出注释状态
                    BEGIN INITIAL; 
                return T_comment_last;}



{RESERVED_WORD}      { 
                        if (strcmp(yytext, "if") == 0) return T_If;
                        if (strcmp(yytext, "else") == 0) return T_Else;
                        if (strcmp(yytext, "for") == 0) return T_For;
                        if (strcmp(yytext, "while") == 0) return T_While;
                        if (strcmp(yytext, "do") == 0) return T_Do;
                        if (strcmp(yytext, "int") == 0) return T_Int;
                        if (strcmp(yytext, "write") == 0) return T_Write;
                        if (strcmp(yytext, "read") == 0) return T_Read;
                        if (strcmp(yytext, "switch") == 0) return T_Switch;
                        if (strcmp(yytext, "case") == 0) return T_Case;
                        if (strcmp(yytext, "break") == 0) return T_Break;
                        if (strcmp(yytext, "default") == 0) return T_Default;
                      }

{DOUBLEWORD}         { 
                        if (strcmp(yytext, ">=") == 0) return T_Ge; 
                        if (strcmp(yytext, "<=") == 0) return T_Le; 
                        if (strcmp(yytext, "!=") == 0) return T_Ne; 
                        if (strcmp(yytext, "==") == 0) return T_Eq; 
                        if (strcmp(yytext, "&&") == 0) return T_And; 
                        if (strcmp(yytext, "||") == 0) return T_Or;
                      }

{SINGLEWORD}         { return yytext[0]; }    

{NUM}{ID}            {lex_error("error id",cur_line_num);}

{NUM}                { return T_NumConstant; }

{ID}                 { return T_Identifier; }

.                    { lex_error("Unrecognized character", cur_line_num); }

<<EOF>>              { return 0; }
%%
int main(int argc,char* argv[]){
	int token;
	init_scanner();
	while(token = yylex()){
		print_token(token);
		puts(yytext);
	}
	return 0;
}

void init_scanner(){
	printf("%-20s%s\n","TOKEN-TYPE","TOKEN-VALUE");
}

void lex_error(char* msg,int line){
printf("Error at line %-3d:%s '%s'\n",line,msg,yytext);
}
int yywrap(void){
return 1;
}

%{
    #include<stdio.h>
    #include"node.h"
    #include"lex.yy.c"
    #include"symbol.h"
	
    extern int synError;
    pNode root;
    #define YYERROR_VERBOSE 1
    extern int yylineno;
    extern int yy_flex_debug;
    extern FILE *yyin;
    extern FILE *yyout;
    void  yyerror(const char *msg);
%}


// types
%union{
    pNode node; 
}

// tokens
%token FUNCTION MAIN
%token <node> RELOP
%token <node> NUM 
%token <node> ID
%token <node> TYPE
%token <node> PLUS MINUS STAR DIV
%token <node> AND OR NOT NOTE EQ GE LE ASSIGNOP 
%token <node> LP RP SEMI COMMA BIG LET LC RC 
%token <node> IF ELSE FOR WHILE DO INT WRITE READ SWITCH CASE BREAK DEFAULT CALL
%token <node> COMMENT_FIRST COMMENT_LAST

// non-terminals
%type <node> Program FunDeclaration MainDeclaration FunctionBody DeclarationList StatementList Statement
%type <node> DeclarationStat IfStat WhileStat ForStat ReadStat WriteStat CompoundStat ExpressionStat CallStat Expression BoolExpr AdditiveExpr Term Factor TermTail  ElseStat   AdditiveExprTail
// precedence and associativity

%right ASSIGNOP
%left OR
%left AND
%left NOTE EQ GE LE  BIG LET
%left PLUS MINUS
%left STAR DIV
%right NOT
%left LC RC
%left LP RP
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE


%%

// Program Rule: A program is a fun_declaration followed by main_declaration
Program:            FunDeclaration MainDeclaration
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "Program", 2, $1, $2); root = $$;
//printTreeInfo(root, 0); 
}
    ;

// Function Declaration: function ID ( ) function_body
FunDeclaration:     FUNCTION ID LP RP FunctionBody
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "FunDeclaration", 4, $2, $3, $4, $5); }
    ;

// Main Declaration: main ( ) function_body
MainDeclaration:    MAIN LP RP FunctionBody
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "MainDeclaration", 3, $2, $3, $4); }
    ;

// Function Body: { declaration_list statement_list }
FunctionBody:       LC DeclarationList StatementList RC
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "FunctionBody", 4, $1, $2, $3, $4); }
    ;

// Declaration List: { declaration_list } | ε
DeclarationList:    DeclarationStat DeclarationList
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "DeclarationList", 2, $1, $2); }
    |                 { $$ = NULL; }
    ;

// Declaration Stat: int ID ;
DeclarationStat:    INT ID SEMI
                  { $$ = newNode(@$.first_line, NODE_DECLARATION, "DeclarationStat", 3, $1, $2,$3); }
    ;

// Statement List: { statement_list } | ε
StatementList:      Statement StatementList
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "StatementList", 2, $1, $2); }
    |                 { $$ = NULL; }
    ;

// Statement: <if_stat> | <while_stat> | <for_stat> | <read_stat> 
//           | <write_stat> | <compound_stat> | <expression_stat> | <call_stat>
Statement:          IfStat
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "Statement", 1, $1); }
    |               WhileStat
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "Statement", 1, $1); }
    |               ForStat
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "Statement", 1, $1); }
    |               ReadStat
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "Statement", 1, $1); }
    |               WriteStat
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "Statement", 1, $1); }
    |               CompoundStat
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "Statement", 1, $1); }
    |               ExpressionStat
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "Statement", 1, $1); }
    |               CallStat
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "Statement", 1, $1); }
    ;

// If Statement: if ( expr ) statement [ else statement ]
IfStat:            IF LP Expression RP Statement ElseStat
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "IfStat", 6, $1, $2, $3, $4, $5,$6); }
    ;

ElseStat:          ELSE Statement
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "ElseStat", 2, $1, $2); }
    |               { $$ = NULL; }
    ;

// While Statement: while ( bool_expr ) statement
WhileStat:         WHILE LP BoolExpr RP Statement
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "WhileStat", 5, $1, $2, $3, $4,$5); }
    ;

// For Statement: for ( expression ; bool_expr ; expression ) statement
ForStat:           FOR LP Expression SEMI BoolExpr SEMI Expression RP Statement
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "ForStat", 9, $1, $2, $3, $4, $5, $6, $7,$8,$9); }
    ;

// Write Statement: write expression ;
WriteStat:         WRITE Expression SEMI
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "WriteStat",3, $1, $2,$3); }
    ;

// Read Statement: read ID ;
ReadStat:          READ ID SEMI
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "ReadStat", 3, $1, $2,$3); }
    ;

// Compound Statement: { statement_list }
CompoundStat:      LC StatementList RC
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "CompoundStat", 3, $1, $2,$3); }
    ;

// Expression Statement: expression ; | ;
ExpressionStat:    Expression SEMI
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "ExpressionStat", 2, $1, $2); }
    |               SEMI
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "ExpressionStat", 1); }
    ;

// Call Statement: call ID ( );
CallStat:          CALL ID LP RP SEMI
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "CallStat", 5, $1, $2, $3, $4,$5); }
    ;

// Expression: ID = bool_expr | bool_expr
Expression:        ID ASSIGNOP BoolExpr
                  { $$ = newNode(@$.first_line, NODE_ASSIGNMENT, "Expression", 3, $1, $2, $3); }
    |               BoolExpr
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "Expression", 1, $1); }
    ;

// Bool Expression: additive_expr | additive_expr ( > | < | >= | <= | == | != ) additive_expr
BoolExpr:          AdditiveExpr
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "BoolExpr", 1, $1); }
    |               AdditiveExpr RELOP AdditiveExpr
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "BoolExpr", 3, $1, $2, $3); }
    ;

// Additive Expression: term { (+ | -) term }
AdditiveExpr:      Term AdditiveExprTail
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "AdditiveExpr", 2, $1, $2); }
    ;

AdditiveExprTail:   PLUS Term AdditiveExprTail
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "AdditiveExprTail", 3, $1, $2, $3); }
    |               MINUS Term AdditiveExprTail
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "AdditiveExprTail", 3, $1, $2, $3); }
    |               { $$ = NULL; }
    ;

// Term: factor { (* | /) factor }
Term:              Factor TermTail
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "Term", 2, $1, $2); }
    ;

TermTail:          STAR Factor TermTail
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "TermTail", 3, $1, $2, $3); }
    |               DIV Factor TermTail
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "TermTail", 3, $1, $2, $3); }
    |               { $$ = NULL; }
    ;

// Factor: ( additive_expr ) | ID | NUM
Factor:            LP AdditiveExpr RP
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "Factor", 3, $1, $2, $3); }
    |               ID
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "Factor", 1, $1); }
    |               NUM
                  { $$ = newNode(@$.first_line, NOT_A_TOKEN, "Factor", 1, $1); }
    ;
%%

void yyerror(const char* msg){
    fprintf(stderr, "Error type B at line %d: %s.\n", yylineno, msg);
}



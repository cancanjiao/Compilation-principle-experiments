#ifndef ENUM_H
#define ENUM_H

// define node type
typedef enum nodeType {
    TOKEN_NUM,
    TOKEN_ID,
    TOKEN_TYPE,
    NODE_DECLARATION,
    NODE_ASSIGNMENT,
    NODE_EXPRESSION, 
    // TOKEN_COMMA,
    // TOKEN_SEMI,
    // TOKEN_ASSIGNOP,
    // TOKEN_RELOP,
    // TOKEN_PLUS,
    // TOKEN_MINUS,
    TOKEN_OTHER,
    NOT_A_TOKEN

} NodeType;
#endif

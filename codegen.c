#include "node.h"
#include <stdio.h>
#include <string.h>
#include "symbol.h"
#include "enum.h"
// 预声明 generate_expression
void generate_expression(pNode expr);

typedef struct Code {
    char opt[10];  // 操作码
    int operand;   // 操作数（地址或者常量值）
} Code;

Code codeList[100];
int codeIndex = 0;
int flag = 0;
int flagaddress;
 SymbolTable * currentScope = NULL;

// 插入中间代码指令
void insert_Instruction(char op[10], int operand) {
    strcpy(codeList[codeIndex].opt, op);
    codeList[codeIndex].operand = operand;
    codeIndex++;
}

// 生成函数声明的中间代码
void generate_function_declaration(pNode func_decl) {
    insert_Instruction("ENTER", 3);
    SymbolTable *newScope = createSymbolTable();
     if (newScope == NULL)
    {
      printf ("错误：创建新符号表失败。\n");
      return;
    }
    newScope->parent = currentScope;  // 确保 currentScope 和 newScope->parent 类型一致
    currentScope = newScope;
    currentScope->symbolnum = 0;
}

// 生成主函数的中间代码
void generate_main_declaration(pNode main_decl) {
    insert_Instruction("RETURN", 0);

    int num = 0;
    for (int i = 0; i < codeIndex; i++) {
	    num++;
    }
   codeList[0].operand = num; 
    insert_Instruction("ENTER", 3);
    SymbolTable *newScope = createSymbolTable();
 if (newScope == NULL)
    {
      printf ("错误：创建新符号表失败。\n");
      return;
    }
 newScope->parent = currentScope;  // 确保 currentScope 和 newScope->parent 类型一致
    currentScope = newScope;
    currentScope->symbolnum = 0;
}

// 生成变量声明的中间代码
void generate_variable_declaration(pNode var_decl) {
    int address = currentScope->symbolnum + 2;
    VarType type =  TYPE_INT;
  //  printf(" %s %d \n",var_decl->child->next->val,address);
    addSymbol(currentScope, var_decl->child->next->val, address, type);  // 使用正确的参数调用 addSymbol
   // insert_Instruction("STO", address);
    currentScope->symbolnum++;
    //printSymbolTable(currentScope);
}

// 生成赋值语句的中间代码
void generate_assignment(pNode assign_stmt) {
    int address = getAddress(currentScope, assign_stmt->child->val);
//    printf("%s\n",assign_stmt->child->val);
    generate_expression(assign_stmt->child->next);  // 右侧的表达式
   flag = 1; 
   flagaddress = address;
    //insert_Instruction("STO", address);
}

// 生成读取语句的中间代码
void generate_read_statements(pNode io_stmt) {
 //   printf("%s\n",io_stmt->child->next->val);
    int address = getAddress(currentScope, io_stmt->child->next->val);
    insert_Instruction("IN", 0);
    insert_Instruction("STO", address);
}

// 生成输出语句的中间代码
void generate_write_statements(pNode io_stmt) {
   // printf("%s\n",io_stmt->child->next->val);
    int address = getAddress(currentScope, io_stmt->child->next->val);
    insert_Instruction("LOAD", address);
    insert_Instruction("OUT", 0);
}

// 生成表达式的中间代码
void generate_expression(pNode expr) {
    if (expr->type == TOKEN_ID) {
        int address = getAddress(currentScope, expr->val);
        insert_Instruction("LOAD", address);  // 加载变量值到栈
    } else if (expr->type == NODE_PLUS) {  // 加法表达式
        generate_expression(expr->child);  // 左侧表达式
        generate_expression(expr->child->next);  // 右侧表达式
        insert_Instruction("ADD", 0);  // 执行加法
    } else if (expr->type == NODE_MINUS) {  // 减法表达式
        generate_expression(expr->child);  // 左侧表达式
        generate_expression(expr->child->next);  // 右侧表达式
        insert_Instruction("SUB", 0);  // 执行减法
    } else if (expr->type == NODE_RELOP) {  // 关系运算符（如等于、大于等）
        generate_expression(expr->child);
        generate_expression(expr->child->next);
        if (expr->val == 1) {  // 假设1表示等于关系
            insert_Instruction("EQ", 0);
        } else if (expr->val == 2) {  // 假设2表示不等于
            insert_Instruction("NOTEQ", 0);
        } else if (expr->val == 3) {  // 假设3表示大于
            insert_Instruction("GT", 0);
        } else if (expr->val == 4) {  // 假设4表示小于
            insert_Instruction("LES", 0);
        }
    }
}

// 生成中间代码的主函数
void generate_code(pNode root) {
    if (root == NULL) return;

	    int n;
//    printSymbolTable(currentScope);
    
    switch (root->type) {
        case NODE_FUNCTION:
            generate_function_declaration(root);
            break;
        case NODE_MAIN:
            generate_main_declaration(root);
            break;
        case NODE_DECLARATION:
            generate_variable_declaration(root);
            break;
        case NODE_ASSIGNMENT:
            generate_assignment(root);
            break;
        case NODE_READ:
            generate_read_statements(root);
            break;
        case NODE_WRITE:
            generate_write_statements(root);
            break;
	case TOKEN_NUM: 
	   n  = atoi(root->val );
        	insert_Instruction("LOADI",n);  // 加载常量到栈
    	if(flag == 1)insert_Instruction("STO", flagaddress);
    		break; 
        default:
            break;
    }

    generate_code(root->child);
    generate_code(root->next);
}

// 输出生成的中间代码
void print_code() {
    for (int i = 0; i < codeIndex; i++) {
        printf("%s %d\n", codeList[i].opt, codeList[i].operand);
    }
}


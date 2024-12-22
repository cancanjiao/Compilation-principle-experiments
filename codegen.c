#include "node.h"
#include <stdio.h>
#include <string.h>

// 声明帮助函数
void generate_code (pNode root);
void generate_function_declaration (pNode func_decl);
void generate_main_declaration (pNode main_decl);
void generate_variable_declaration (pNode var_decl);
void generate_assignment (pNode assign_stmt);
void generate_io_statements (pNode io_stmt);
void generate_additive_expression (pNode add_expr);
void generate_boolean_expression (pNode bool_expr);

 // 生成目标代码的主函数
void
generate_code (pNode root)
{
  if (root == NULL)
    return;

  switch (root->type)
    {
    case NODE_FUNCTION:
      generate_function_declaration (root);
      break;
    case NODE_MAIN:
      generate_main_declaration (root);
      break;
    case NODE_DECLARATION:
      generate_variable_declaration (root);
      break;
    case NODE_ASSIGNMENT:
      generate_assignment (root);
      break;
    case NODE_USE:
      generate_io_statements (root);
      break;
    case NODE_ADD:
      generate_additive_expression (root);
      break;
    case NODE_BOOL_EXPR:
      generate_boolean_expression (root);
      break;
    case TOKEN_NUM :
      printf("LOADI %s\n",root->val);
      break;
    default:
      break;
    }

  // 递归处理子节点和兄弟节点
  generate_code (root->child);
  generate_code (root->next);
}

																																	     // 生成函数声明的目标代码
void
generate_function_declaration (pNode func_decl)
{
  printf ("ENTER %s\n", func_decl->name);	// 假设函数有一个名称
  generate_code (func_decl->child);	// 递归生成函数体的代码
  printf ("RETURN\n");
}

																																			 // 生成主函数的目标代码
void
generate_main_declaration (pNode main_decl)
{
  printf ("ENTER main\n");
  generate_code (main_decl->child);	// 递归生成主函数体的代码
  printf ("RETURN\n");
}

																																				     // 生成变量声明的目标代码
void
generate_variable_declaration (pNode var_decl)
{
  printf ("DECLARE %s\n", var_decl->name);	// 假设节点中包含变量的名称
}

																																					 // 生成赋值语句的目标代码
void
generate_assignment (pNode assign_stmt)
{
  pNode var_node = assign_stmt->child;	// 左操作数（变量）
  pNode expr_node = var_node->next;	// 右操作数（表达式）

  generate_code (expr_node);	// 生成右操作数的代码
  printf ("STO %s\n", var_node->val);	// 存储结果到变量
}

																																							 // 生成输入输出语句的目标代码
void
generate_io_statements (pNode io_stmt)
{
  if (strcmp (io_stmt->name, "READ") == 0)
    {
      printf ("IN\n");
      printf ("STO %s\n", io_stmt->child->val);	// 假设节点存储变量名
    }
  else if (strcmp (io_stmt->name, "WRITE") == 0)
    {
      generate_code (io_stmt->child);	// 生成表达式代码
      printf ("OUT\n");
    }
}

																																												     // 生成加法表达式的目标代码
void
generate_additive_expression (pNode add_expr)
{
  pNode left = add_expr->child;	// 左操作数
  pNode right = left->next;	// 右操作数

  generate_code (left);		// 生成左操作数的代码
  generate_code (right);	// 生成右操作数的代码
  printf ("ADD\n");		// 生成加法指令
}

																																															 // 生成布尔表达式的目标代码
void
generate_boolean_expression (pNode bool_expr)
{
  if (bool_expr->child->next)
    {
      pNode left = bool_expr->child;
      pNode relop = left->next;
      pNode right = relop->next;

      generate_code (left);	// 生成左操作数
      generate_code (right);	// 生成右操作数

      if (strcmp (relop->val, "==") == 0)
	{
	  printf ("EQ\n");
	}
      else if (strcmp (relop->val, "!=") == 0)
	{
	  printf ("NOTEQ\n");
	}
      else if (strcmp (relop->val, ">") == 0)
	{
	  printf ("GT\n");
	}
      else if (strcmp (relop->val, "<") == 0)
	{
	  printf ("LES\n");
	}
      else if (strcmp (relop->val, ">=") == 0)
	{
	  printf ("GE\n");
	}
      else if (strcmp (relop->val, "<=") == 0)
	{
	  printf ("LE\n");
	}
    }
  else
    {
//      generate_additive_expression (bool_expr->child);	// 只是一个加法表达式
    }
}


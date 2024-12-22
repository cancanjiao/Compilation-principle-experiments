#include "node.h"
#include "symbol.h"
#include <stdio.h>

// 全局变量，指向当前的作用域
SymbolTable *currentScope = NULL;

void
enterScope ()
{
  SymbolTable *newScope = createSymbolTable();
  newScope->parent = currentScope;
  currentScope = newScope;
}

void
exitScope ()
{
  if (currentScope)
    {
      SymbolTable* parentScope = currentScope->parent;
      destroySymbolTable(currentScope);
      currentScope = parentScope;
    }
}

void
checkDeclaration (pNode node)
{
  if (findSymbol (currentScope, node->child->val) == NULL)
    {
      addSymbol (currentScope, node->child->val, TYPE_INT);
    }
  else
    {
      printf ("Error: Variable '%s' already declared at line %d\n",
	      node->child->val, node->lineNo);
    }
}

void
checkAssignment (pNode node)
{
  if (findSymbol (currentScope, node->child->val) == NULL)
    {
      printf ("Error: Variable '%s' not declared at line %d\n",
	      node->child->val, node->lineNo);
    }
}

void
performSemanticAnalysis (pNode root)
{
  if (!root)
    return;
  switch (root->type)
    {
    case NODE_DECLARATION:
      //printf ("Semantic analysis: Declaration at line %d\n", root->lineNo);
      checkDeclaration (root);
      break;
    case NODE_ASSIGNMENT:
      //printf ("Semantic analysis: Assignment at line %d\n", root->lineNo);
      checkAssignment (root);
      break;
    case NODE_EXPRESSION:
      //printf ("Semantic analysis: Expression at line %d\n", root->lineNo);
      // 表达式语义分析
      break;
    default:
      //printf ("Semantic analysis: Unknown node type at line %d\n", root->lineNo);
      break;
    }
  performSemanticAnalysis (root->child);
  performSemanticAnalysis (root->next);
}

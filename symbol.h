#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOLS 1024

typedef enum
{
  TYPE_INT
} VarType;

     // 符号表中的符号
typedef struct Symbol
{
  char* name;			// 变量名
  VarType type;			// 变量类型
  struct Symbol* next;		// 链表结构
} Symbol;

		 // 符号表
typedef struct
{
	struct SymbolTable* parent;
  Symbol *symbols[MAX_SYMBOLS];	// 简单的哈希表
} SymbolTable;

		     // 符号表操作
SymbolTable *createSymbolTable ();
void destroySymbolTable (SymbolTable * table);
unsigned hash (char *str);
Symbol *findSymbol (SymbolTable * table, char *name);
void addSymbol (SymbolTable * table, char *name, VarType type);
void printSymbolTable (SymbolTable * table);

#endif

#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 变量的类型定义
#define INT_TYPE 1
#define FLOAT_TYPE 2
#define STRING_TYPE 3

 // 符号结构体
typedef struct Symbol
{
  char *name;			// 变量名
  int type;			// 变量类型
  int initialized;		// 是否已初始化
} Symbol;

	     // 符号表结构体
typedef struct SymbolTable
{
  Symbol *symbols;		// 符号数组
  int size;			// 当前符号表大小
  int capacity;			// 符号表容量
} SymbolTable;

			 // 符号表的函数声明
SymbolTable *createSymbolTable (int capacity);
void insertSymbol (SymbolTable * table, const char *name, int type);
Symbol *lookupSymbol (SymbolTable * table, const char *name);
void freeSymbolTable (SymbolTable * table);
void expandSymbolTable (SymbolTable * table);

#endif

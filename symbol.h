#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOLS 100 

typedef enum
{
  TYPE_INT
} VarType;

// 符号表中的符号
typedef struct Symbol
{
  char* name;             // 变量名
  struct Symbol* next;    // 链表结构
  int address;            // 栈中的地址
  VarType type;           // 类型
} Symbol;

// 符号表
typedef struct
{
  struct SymbolTable* parent;    // 父符号表
  Symbol *symbols[MAX_SYMBOLS];  // 简单的哈希表
  int symbolnum;                 // 当前符号表中符号数量
} SymbolTable;

// 符号表操作
SymbolTable *createSymbolTable();
void destroySymbolTable(SymbolTable *table);
unsigned hash(char *str);
Symbol *findSymbol(SymbolTable *table, char *name);
void addSymbol(SymbolTable *table, char *name, int symboladdress, VarType type);
void printSymbolTable(SymbolTable *table);
int getAddress(SymbolTable *table, char *name);
#endif


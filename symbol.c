#include "symbol.h"

// 创建符号表
SymbolTable *
createSymbolTable (int capacity)
{
  SymbolTable *table = (SymbolTable *) malloc (sizeof (SymbolTable));
  table->symbols = (Symbol *) malloc (sizeof (Symbol) * capacity);
  table->size = 0;
  table->capacity = capacity;
  return table;
}

		     // 扩展符号表
void
expandSymbolTable (SymbolTable * table)
{
  table->capacity *= 2;
  table->symbols =
    (Symbol *) realloc (table->symbols, sizeof (Symbol) * table->capacity);
}

			     // 插入符号到符号表
void
insertSymbol (SymbolTable * table, const char *name, int type)
{
  if (table->size == table->capacity)
    {
      expandSymbolTable (table);
    }
  table->symbols[table->size].name = strdup (name);
  table->symbols[table->size].type = type;
  table->symbols[table->size].initialized = 0;
  table->size++;
}

							     // 查找符号表中的符号
Symbol *
lookupSymbol (SymbolTable * table, const char *name)
{
  for (int i = 0; i < table->size; i++)
    {
      if (strcmp (table->symbols[i].name, name) == 0)
	{
	  return &table->symbols[i];
	}
    }
  return NULL;			// 没有找到符号
}

												     // 释放符号表占用的内存
void
freeSymbolTable (SymbolTable * table)
{
  for (int i = 0; i < table->size; i++)
    {
      free (table->symbols[i].name);	// 释放每个符号名的内存
    }
  free (table->symbols);	// 释放符号数组的内存
  free (table);			// 释放符号表结构体的内存
}

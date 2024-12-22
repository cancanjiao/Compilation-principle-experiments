#include "symbol.h"
#include <string.h>

char *strdup(const char *str) {
	    size_t len = strlen(str) + 1;
	        char *copy = malloc(len);
		    if (copy) {
			            memcpy(copy, str, len);
				        }
		        return copy;
}


unsigned hash (char *str)
{
  unsigned hash = 0;
  while (*str)
    {
      hash = (hash * 31) + *str++;
    }
  return hash % MAX_SYMBOLS;
}

SymbolTable * createSymbolTable ()
{
  SymbolTable *table = (SymbolTable *) malloc (sizeof (SymbolTable));
  for (int i = 0; i < MAX_SYMBOLS; i++)
    {
      table->symbols[i] = NULL;
    }
  return table;
}

void
destroySymbolTable (SymbolTable * table)
{
  for (int i = 0; i < MAX_SYMBOLS; i++)
    {
      Symbol *symbol = table->symbols[i];
      while (symbol)
	{
	  Symbol *temp = symbol;
	  symbol = symbol->next;
	  free (temp->name);
	  free (temp);
	}
    }
  free (table);
}


Symbol *
findSymbol (SymbolTable * table, char *name)
{
  // 检查符号表和名称是否为 NULL
  if (table == NULL || name == NULL)
    {
      printf ("错误：传递给 findSymbol 的指针为 NULL\n");
      return NULL;
    }

  // 计算哈希值，找到对应的符号链表
  unsigned idx = hash (name);
  Symbol *symbol = table->symbols[idx];
  while (symbol)
    {
      if (strcmp (symbol->name, name) == 0)
	{
	  return symbol;
	}
      symbol = symbol->next;
    }
  return NULL;
}
																							     
void
addSymbol (SymbolTable * table, char *name, VarType type)
{
  unsigned idx = hash (name);
  Symbol *symbol = (Symbol *) malloc (sizeof (Symbol));
  symbol->name = strdup (name);
  symbol->type = type;
  symbol->next = table->symbols[idx];
  table->symbols[idx] = symbol;
}

void
printSymbolTable (SymbolTable * table)
{
  for (int i = 0; i < MAX_SYMBOLS; i++)
    {
      Symbol *symbol = table->symbols[i];
      while (symbol)
	{
	  printf ("Symbol: %s, Type: %d\n", symbol->name, symbol->type);
	  symbol = symbol->next;
	}
    }
}

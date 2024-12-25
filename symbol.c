#include "symbol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int symbolNum = 0;

char *strdup(const char *str) {
	    size_t len = strlen(str) + 1;
	        char *copy = malloc(len);
		    if (copy) {
			            memcpy(copy, str, len);
				        }
		        return copy;
}


unsigned hash(char *str)
{
    unsigned hash = 0;
    while (*str)
    {
        hash = (hash * 31) + *str++;
    }
    return hash % MAX_SYMBOLS;
}

SymbolTable *createSymbolTable()
{
    SymbolTable *table = (SymbolTable *)malloc(sizeof(SymbolTable));
    for (int i = 0; i < MAX_SYMBOLS; i++)
    {
        table->symbols[i] = NULL;
    }
    table->symbolnum = 0;
 //   printf("cteate symboltable success!\n");
    return table;
}

void destroySymbolTable(SymbolTable *table)
{
    for (int i = 0; i < MAX_SYMBOLS; i++)
    {
        Symbol *symbol = table->symbols[i];
        while (symbol)
        {
            Symbol *temp = symbol;
            symbol = symbol->next;
            free(temp->name);
            free(temp);
        }
    }
    free(table);
}

void addSymbol(SymbolTable *table, char *name, int symboladdress, VarType type)
{
    unsigned idx = hash(name);
    Symbol *symbol = (Symbol *)malloc(sizeof(Symbol));
    symbol->name = strdup(name);
    symbol->address = symboladdress;
    symbol->type = type;
    symbol->next = table->symbols[idx];
    table->symbols[idx] = symbol;
    table->symbolnum++;
    //printf("add symbol %s success! %d\n",name,symbol->address);
}

Symbol *findSymbol(SymbolTable *table, char *name)
{
    unsigned idx = hash(name);
    Symbol *symbol = table->symbols[idx];
    while (symbol)
    {
        if (strcmp(symbol->name, name) == 0)
        {
    //printf("find symbol %s %s success!\n",symbol->name,name);
            return symbol;
        }
        symbol = symbol->next;
    }
    return NULL;
}

int getAddress(SymbolTable *table, char *name)
{
    Symbol *symbol = findSymbol(table, name);
    //printf("%s",name);
    if (symbol)
    {
//	    printf("get address success %s %d\n",symbol->name,symbol->address);
        return symbol->address;
    }
    return -1; // 代表未找到
}

void printSymbolTable(SymbolTable *table) {
    if (table == NULL) {
        printf("NULL\n");
        return;
    }
    for (int i = 0; i < MAX_SYMBOLS; i++) {
        Symbol *symbol = table->symbols[i];
	if(!symbol){
		continue;
	}
        while (symbol) {
            printf("Symbol: %s, address: %d, type: %d\n", symbol->name, symbol->address, symbol->type);
            symbol = symbol->next;
        }
    }
}

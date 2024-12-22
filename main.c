#include "node.h"
#include "syntax.tab.h"

extern void performSemanticAnalysis (Node * root);

extern pNode root;

extern int yylineno;
extern int yyparse ();
extern void yyrestart (FILE *);

unsigned lexError = FALSE;
unsigned synError = FALSE;

int
main (int argc, char **argv)
{
  printf ("main start\n");
  if (argc <= 1)
    {
      yyparse ();		//解析输入流
      printf ("scanner over\n");
      return 1;
    }
  printf ("file open start");
  FILE *f = fopen (argv[1], "r");
  if (!f)
    {
      perror (argv[1]);
      return 1;
    }
  printf ("yyrestart\n");
  yyrestart (f);
  printf ("syntax is start\n");
  yyparse ();			//进行语法分析
  printf ("syntax is over\n");
  if (root == NULL)
    {
      printf ("error root is null\n");
    }

  if (!lexError && !synError)
    {
      printf ("starting srmantic analysis\n");
      performSemanticAnalysis (root);
      printf ("semantic start\n");
      printTreeInfo (root, 0);
    }
  delNode (root);		//清理语法树
  return 0;
}

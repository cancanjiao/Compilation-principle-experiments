#ifndef TOLEN_H
#define TOKEN_H
typedef enum
{
  T_Ge = 256, T_Le, T_Eq, T_Ne, T_And, T_Or, T_NumConstant,
  T_Identifier, T_If, T_Else, T_For, T_While, T_Do, T_Int,
  T_Write, T_Read, T_Switch, T_Case, T_Break, T_Default,T_comment_first,T_comment_last
} TokenType;

static void
print_token (int token)
{
  static char *token_strs[] = {
    ">=", "<=", "==", "!=", "&&", "||", "T_Num",
    "ID", "If", "Else", "For", "While", "Do", "Int",
    "Write", "Read", "Switch", "Case", "Break", "Default","Comment_first","Comment_last"
  };

  if (token < 256)
    {
      printf ("%-20c", token);	// 打印字符常量
    }
  else
    {
      printf ("%-20s", token_strs[token - 256]);	// 打印关键词
    }
}

#endif

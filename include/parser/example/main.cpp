
extern "C" {
#include "lzhi.tab.h"
extern int yydebug;
extern int yyparse();
}
#include <spdlog/spdlog.h>
#include <stdio.h>
#include <stdlib.h> // malloc, exit
#include <string.h>

extern FILE *yyin;
extern char *file_name;

int main(int ac, char **av) 
{

  if (ac > 1 && !strcmp(av[1], "-d")) 
  {
    yydebug = 1;
    ac--;
    av++;
  }

  if (ac > 1) {
    if ((yyin = fopen(av[1], "r")) == NULL) 
    {
      perror(av[1]);
      exit(1);
    }
    file_name = av[1];
  } else
    file_name = "(stdin)";

  if (!yyparse())
    SPDLOG_INFO("parse success");
  else
    SPDLOG_ERROR("parse failed");

  return 0;
}
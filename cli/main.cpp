#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h> //注意，readline.h中可能需要调用标准IO库的内容，所以stdio.h必须在readline.h之前被包含
#include <stdlib.h>
// delete  ${ledger_name}
// create leadger ${ledger_name} [if not exists] [with type ${type} ${user}]
int com_list(char *para) {
  printf("do com_list:%s\n", para);
  return 0;
}

int com_view(char *para) {
  printf("do com_view:%s\n", para);
  return 0;
}

int com_rename(char *para) {
  printf("do com_rename:%s\n", para);
  return 0;
}
int com_stat(char *para) {
  printf("do com_stat:%s\n", para);
  return 0;
}

int com_pwd(char *para) {
  printf("do com_pwd:%s\n", para);
  return 0;
}

// select
int com_delete(char *para) {
  printf("do com_delete:%s\n", para);
  return 0;
}
int com_help(char *para) {
  printf("do com_help:%s\n", para);
  return 0;
}

int com_cd(char *para) {
  printf("do com_cd:%s\n", para);
  return 0;
}
int com_quit(char *para) {
  printf("do com_quit:%s\n", para);
  exit(0);
}

/*
 * A structure which contains information on the commands this program
 * can understand.
 */
typedef struct {
  char *name;         /* User printable name of the function. */
  rl_icpfunc_t *func; /* Function to call to do the job. */
  char *doc;          /* Documentation for this function. */
} COMMAND;

COMMAND commands[] = {{"cd", com_cd, "Change to directory DIR"},
                      {"delete", com_delete, "Delete FILE"},
                      {"help", com_help, "Display this text"},
                      {"?", com_help, "Synonym for `help'"},
                      {"list", com_list, "List files in DIR"},
                      {"ls", com_list, "Synonym for `list'"},
                      {"pwd", com_pwd, "Print the current working directory"},
                      {"quit", com_quit, "Quit using Fileman"},
                      {"rename", com_rename, "Rename FILE to NEWNAME"},
                      {"stat", com_stat, "Print out statistics on FILE"},
                      {"view", com_view, "View the contents of FILE"},
                      {(char *)NULL, (rl_icpfunc_t *)NULL, (char *)NULL}};

char *dupstr(char *s) {
  char *r;

  r = (char *)malloc(strlen(s) + 1);
  strcpy(r, s);
  return (r);
}

// clear up white spaces
char *stripwhite(char *string) {
  register char *s, *t;

  for (s = string; whitespace(*s); s++)
    ;

  if (*s == 0)
    return (s);

  t = s + strlen(s) - 1;
  while (t > s && whitespace(*t))
    t--;

  *++t = '\0';

  return s;
}

/*
 * Look up NAME as the name of a command, and return a pointer to that
 * command. Return a NULL pointer if NAME isn't a command name.
 */
COMMAND *find_command(char *name) {
  register int i;

  for (i = 0; commands[i].name; i++)
    if (strcmp(name, commands[i].name) == 0)
      return (&commands[i]);

  return ((COMMAND *)NULL);
}

/* Execute a command line. */
int execute_line(char *line) {
  register int i;
  COMMAND *command;
  char *word;

  /* Isolate the command word. */
  i = 0;
  while (line[i] && whitespace(line[i]))
    i++;
  word = line + i;

  while (line[i] && !whitespace(line[i]))
    i++;

  if (line[i])
    line[i++] = '\0';

  command = find_command(word);

  if (!command) {
    fprintf(stderr, "%s: No such command for FileMan.\n", word);
    return (-1);
  }

  /* Get argument to command, if any. */
  while (whitespace(line[i]))
    i++;

  word = line + i;

  /* Call the function. */
  return ((*(command->func))(word));
}

/*
 * Generator function for command completion. STATE lets us know whether
 * to start from scratch; without any state (i.e. STATE == 0), then we
 * start at the top of the list.
 */
char *command_generator(const char *text, int state) {
  static int list_index, len;
  char *name;

  /*
   * If this is a new word to complete, initialize now. This includes
   * saving the length of TEXT for efficiency, and initializing the index
   * variable to 0.
   */
  if (!state) {
    list_index = 0;
    len = strlen(text);
  }

  /* Return the next name which partially matches from the command list. */
  while (name = commands[list_index].name) {
    list_index++;

    if (strncmp(name, text, len) == 0)
      return (dupstr(name));
  }

  /* If no names matched, then return NULL. */
  return ((char *)NULL);
}

/*
 * Attempt to complete on the contents of TEXT. START and END bound the
 * region of rl_line_buffer that contains the word to complete. TEXT is
 * the word to complete. We can use the entire contents of rl_line_buffer
 * in case we want to do some simple parsing. Return the array of matches,
 * or NULL if there aren't any.
 */
char **fileman_completion(const char *text, int start, int end) {
  char **matches;

  matches = (char **)NULL;

  /*
   * If this word is at the start of the line, then it is a command
   * to complete. Otherwise it is the name of a file in the current
   * directory.
   */
  if (start == 0)
    matches = rl_completion_matches(text, command_generator);

  return (matches);
}

/*
 * Tell the GNU Readline library how to complete. We want to try to complete
 * on command names if this is the first word in the line, or on filenames
 * if not.
 */
void initialize_readline() {
  /* Allow conditional parsing of the ~/.inputrc file. */
  rl_readline_name = ">";

  /* Tell the completer that we want a crack first. */
  rl_attempted_completion_function = fileman_completion;
}

int main(int argc, char **argv) {
  char *line, *s;

  initialize_readline(); /* Bind our completer. */

  printf("LedgerDB\n");
  printf("version: %s\n", "1.0.0");
  printf("\n");
  printf("Type `help' for a list of commands.\n");
  printf("\n");

  /* Loop reading and executing lines until the user quits. */
  for (;;) {
    line = readline(">: ");

    if (!line)
      break;

    /*
     * Remove leading and trailing whitespace from the line.
     * Then, if there is anything left, add it to the history list
     * and execute it.
     */
    s = stripwhite(line);
    if (*s) {
      add_history(s);
      execute_line(s);
    }

    free(line);
  }
  exit(0);
}
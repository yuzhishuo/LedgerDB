#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h> //注意，readline.h中可能需要调用标准IO库的内容，所以stdio.h必须在readline.h之前被包含
#include <stdlib.h>
#include <parser/Parser.h>
#include <stdlib.h>  // for

#include <parser/GrammarCommandFactory.h>
#include <CreateLedger.h>

// delete  ${ledger_name}
// create leadger ${ledger_name} [if not exists] [with type ${type} ${user}]
int com_view(char *para) {
  printf("do com_view:%s\n", para);
  return 0;
}

int com_stat(char *para)
{
    auto parser = yuzhi::grammar::Parser::Instance();
    return parser->handle(std::string(para));
}

int com_help(char *para)
{
  printf("do com_help:%s\n", para);
  return 0;
}

int com_quit(char *para)
{
  printf("do com_quit:%s\n", para);
  exit(0);
}

typedef struct {
  const char *name;         /* User printable name of the function. */
  rl_icpfunc_t *func; /* Function to call to do the job. */
  const char *doc;          /* Documentation for this function. */
} COMMAND;

COMMAND commands[] = {
      {"help", com_help, "Display this text"},
      {"?", com_help, "Synonym for `help'"},
      {"stat", com_stat, "Print out statistics on FILE"},
      {"view", com_view, "View the contents of FILE"},
      {"quit", com_quit, "quit programs"}
};

char *dupstr(const char *s) {
  char *r;

  r = (char *)malloc(strlen(s) + 1);
  strcpy(r, s);
  return (r);
}

// clear up white spaces
char *stripwhite(char *string) {
  char *s, *t;

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
COMMAND *find_command(const std::string& word) {

  for (auto i = 0; commands[i].name; i++)
    if (strcmp(word.c_str(), commands[i].name) == 0)
      return (&commands[i]);

  return ((COMMAND *)NULL);
}

/* Execute a command line. */
int execute_line(char *line) {
  int i;
  COMMAND *command;
  char *ptr;

  /* Isolate the command word. */
  i = 0;
  while (line[i] && whitespace(line[i]))
    i++;
    ptr = line + i;
    while (line[i] && !whitespace(line[i]))
      i++;


  std::string word = std::string(ptr, i);
  command = find_command(word);

  if (!command) {
      return com_stat(line);
  }

  while (whitespace(line[i]))
    i++;

  word = line + i;

  /* Call the function. */
  return ((*(command->func))(word.data()));
}

/*
 * Generator function for command completion. STATE lets us know whether
 * to start from scratch; without any state (i.e. STATE == 0), then we
 * start at the top of the list.
 */
char *command_generator(const char *text, int state) {
  static int list_index, len;
  const char *name;

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

void parse_int ()
{
  using yuzhi::grammar::GrammarCommandFactory;

  auto& factory = GrammarCommandFactory::Instance();

  factory.add(std::make_shared<yuzhi::grammar::cli::CreateLedger>());
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  parse_int();

  char *line, *s;

  initialize_readline();

  printf("LedgerDB\n");
  printf("version: %s\n", "1.0.0");
  printf("\n");
  printf("Type `help' for a list of commands.\n");
  printf("\n");

  while (true)
  {
    line = readline(">: ");

    if (!line)
      break;
    s = stripwhite(line);
    if (*s)
    {
      add_history(s);
      execute_line(s);
    }

    free(line);
  }
  exit(0);
}
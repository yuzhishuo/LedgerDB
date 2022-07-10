%{
    #include<stdio.h>
    #include<stdlib.h>
    #include<string.h>
    #include<stdarg.h> // for va_list, va_start, va_end, va_arg
    extern void emit(char *s, ...);
    extern int yylex (void);
    char *file_name;
%}

%code requires {

    extern char *file_name;
    typedef struct YYLTYPE {
    int first_line;
    int first_column;
    int last_line;
    int last_column;
    char *file_name;
    } YYLTYPE;
    # define YYLTYPE_IS_DECLARED 1

    # define YYLLOC_DEFAULT(Current, Rhs, N)				\
        do									\
        if (N)                                                            \
        {								\
        (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
        (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
        (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
        (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
        (Current).file_name     = YYRHSLOC (Rhs, 1).file_name;	        \
        }								\
        else								\
        { /* empty RHS */						\
        (Current).first_line   = (Current).last_line   =		\
            YYRHSLOC (Rhs, 0).last_line;				\
        (Current).first_column = (Current).last_column =		\
            YYRHSLOC (Rhs, 0).last_column;				\
        (Current).file_name  = NULL;					\
        }								\
        while (0)
}

/* names and literal values */

%union {
    int intval;
    double floatval;
    char *strval;
    int subtok;
}

%token <strval> NAME /* like ledger_name, user_name, etc. */ 
%token <strval> STRING  
%token <intval> INTNUM
%token <intval> BOOL

%token CREATE
%token LEDGER
%token USER
%token GRANT
%token IF
%token NOT
%token WITH
%token OWN
%token COMMENT
%token <subtok> EXISTS


%type <intval> opt_if_not_exists grant_list grant_item

%start stmt_list

%{
void yyerror(char *s, ...);
void lyyerror(YYLTYPE, char *s, ...);
%}

  /* free discarded tokens */
%destructor { printf ("free at %d %s\n",@$.first_line, $$); free($$); } <strval>

%%


stmt_list: stmt_list stmt ';'
        | stmt ';'
        ;

stmt_list: error ';' 
        | stmt_list error ';'
        ;

/* create */
stmt: create_stmt { emit("STMT"); }
    ;

create_stmt: CREATE LEDGER opt_if_not_exists NAME  { emit("CREATE_LEDGER", $3, $4); free($4); } ;
            | CREATE LEDGER opt_if_not_exists NAME WITH GRANT  '(' grant_list ')' { emit("CREATE_LEDGER", $3, $4, $8); free($4); } ;

opt_if_not_exists:  /* nil */ { $$ = 0; }
   | IF EXISTS        { if(!$2) { lyyerror(@2,"IF EXISTS doesn't exist"); YYERROR; }
                        $$ = $2; /* NOT EXISTS hack */ }
   ;

grant_list: NAME grant_item { emit("GRANT_LIST %s %d", $1, $2); free($1); $$ = 1; }
       /* | NAME grant_item ',' grant_list { emit("GRANT_LIST %s %d", $1, $2); free($1); $$ = $4 + 1;} */
        ;

grant_item: OWN  {  $$=1; }
        | COMMENT {  $$=2; }
        ;

%%



void yyerror(char *s, ...)
{
  va_list ap;
  va_start(ap, s);

  if(yylloc.first_line)
    fprintf(stderr, "%s:%d.%d-%d.%d: error: ", yylloc.file_name, yylloc.first_line, yylloc.first_column,
	    yylloc.last_line, yylloc.last_column);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");

}

void lyyerror(YYLTYPE t, char *s, ...)
{
  va_list ap;
  va_start(ap, s);

  if(t.first_line)
    fprintf(stderr, "%s:%d.%d-%d.%d: error: ", t.file_name, t.first_line, t.first_column,
	    t.last_line, t.last_column);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}
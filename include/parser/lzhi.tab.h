/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_HOME_YUZHI_PROJECTS_LEDGER_INCLUDE_PARSER_LZHI_TAB_H_INCLUDED
# define YY_YY_HOME_YUZHI_PROJECTS_LEDGER_INCLUDE_PARSER_LZHI_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 10 "/home/yuzhi/projects/ledger/include/parser/zhi.yy"


    char *file_name;

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

#line 82 "/home/yuzhi/projects/ledger/include/parser/lzhi.tab.h"

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NAME = 258,
    STRING = 259,
    INTNUM = 260,
    BOOL = 261,
    CREATE = 262,
    LEDGER = 263,
    USER = 264,
    GRANT = 265,
    IF = 266,
    NOT = 267,
    WITH = 268,
    OWN = 269,
    COMMENT = 270,
    EXISTS = 271
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 46 "/home/yuzhi/projects/ledger/include/parser/zhi.yy"

    int intval;
    double floatval;
    char *strval;
    int subtok;

#line 117 "/home/yuzhi/projects/ledger/include/parser/lzhi.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_HOME_YUZHI_PROJECTS_LEDGER_INCLUDE_PARSER_LZHI_TAB_H_INCLUDED  */

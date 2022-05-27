%option noyywrap nodefault yylineno case-insensitive
%{
#include <lzhi.tab.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

void yyerror(char *s, ...);
int oldstate;

/* handle locations */
int yycolumn = 1;

#define YY_USER_ACTION yylloc.file_name = file_name; \
    yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; yylloc.last_column = yycolumn+yyleng-1; \
    yycolumn += yyleng;

%}


%x COMMENT

%%

CREATE { return CREATE; }
LEDGER {return LEDGER; }
USER { return USER; }
GRANT { return GRANT; }
IF { return IF; }
NOT { return NOT; }
WITH { return WITH; }
OWN { return OWN; }
COMMENT { return COMMENT; }
EXISTS	{ yylval.subtok = 0; return EXISTS; }
NOT[ \t]+EXISTS	{ yylval.subtok = 1; return EXISTS; }


[-+&~|^/%*(),.;!]   { return yytext[0]; }

'(\\.|''|[^'\n])*'	|
\"(\\.|\"\"|[^"\n])*\"  { yylval.strval = strdup(yytext); return STRING; }

'(\\.|[^'\n])*$	        { yyerror("Unterminated string %s", yytext); }
\"(\\.|[^"\n])*$	{ yyerror("Unterminated string %s", yytext); }



[A-Za-z][A-Za-z0-9_]*	{ yylval.strval = strdup(yytext);
                          return NAME; }
`[^`/\\.\n]+`           { yylval.strval = strdup(yytext+1);
                          yylval.strval[yyleng-2] = 0;
                          return NAME; }

`[^`\n]*$               { yyerror("unterminated quoted name %s", yytext); }


 
#.*		;
"--"[ \t].*	;

"/*"            { oldstate = YY_START; BEGIN COMMENT; }
<COMMENT>"*/"   { BEGIN oldstate; }
<COMMENT>.   ;
<COMMENT>\n     { yycolumn = 1; }
<COMMENT><<EOF>> { yyerror("unclosed comment"); }


[ \t]           /* white space */
\n              { yycolumn = 1; }
.               { yyerror("mystery character '%c'", *yytext); }


%%
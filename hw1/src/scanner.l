%{
#include <stdint.h>

#define LIST                concatenateString(yytext)
#define TOKEN(t)            { LIST; if (opt_tok) printf("<%s>\n", #t); }
#define TOKEN_CHAR(t)       { LIST; if (opt_tok) printf("<%c>\n", (t)); }
#define TOKEN_STRING(t, s)  { LIST; if (opt_tok) printf("<%s: %s>\n", #t, (s)); }
#define MAX_LINE_LENG       512

uint32_t line_num = 1;
static char buffer[MAX_LINE_LENG];

static uint32_t opt_src = 1;
static uint32_t opt_tok = 1;
static char *buffer_ptr = buffer;

static void concatenateString(const char *yytext_ptr);

%}

letter [A-Za-z]
digit [0-9]

identifier {letter}({letter}|{digit})*

dec_num 0|([1-9]{digit}*)
oct_num 0[0-7]*
float_num {dec_num}\.(0|{digit}*[1-9])

nonzero_dec [1-9]{digit}*
nonzero_float ({nonzero_dec}\.(0|{digit}*[1-9]))|({dec_num}\.({digit}*[1-9]))
scientific_num ({nonzero_dec}|{nonzero_float})[eE][+-]?{dec_num}

string_const \"([^\"]|\"{2})*\"

pseudocomments "//&"[A-Z][\+\-].*

%x C_COMMENT
%x CPP_COMMENT

%%
    /* Delimiters */
"," { TOKEN_CHAR(','); }
";" { TOKEN_CHAR(';'); }
":" { TOKEN_CHAR(':'); }
"(" { TOKEN_CHAR('('); }
")" { TOKEN_CHAR(')'); }
"[" { TOKEN_CHAR('['); }
"]" { TOKEN_CHAR(']'); }

    /* Arithmetic, Relational, and Logical Operators */
"+" { TOKEN(+); }
"-" { TOKEN(-); }
"*" { TOKEN(*); }
"/" { TOKEN(/); }
"mod" { TOKEN(mod); }
":=" { TOKEN(:=); }
"<" { TOKEN(<); }
"<=" { TOKEN(<=); }
"<>" { TOKEN(<>); }
">=" { TOKEN(>=); }
">" { TOKEN(>); }
"=" { TOKEN(=); }
"and" { TOKEN(and); }
"or" { TOKEN(or); }
"not" { TOKEN(not); }

    /* Reserved Words */
"var" { TOKEN(KWvar); }
"def" { TOKEN(KWdef); }
"array" { TOKEN(KWarray); }
"of" { TOKEN(KWof); }
"boolean" { TOKEN(KWboolean); }
"integer" { TOKEN(KWinteger); }
"real" { TOKEN(KWreal); }
"string" { TOKEN(KWstring); }
"true" { TOKEN(KWtrue); }
"false" { TOKEN(KWfalse); }
"while" { TOKEN(KWwhile); }
"do" { TOKEN(KWdo); }
"if" { TOKEN(KWif); }
"then" { TOKEN(KWthen); }
"else" { TOKEN(KWelse); }
"for" { TOKEN(KWfor); }
"to" { TOKEN(KWto); }
"begin" { TOKEN(KWbegin); }
"end" { TOKEN(KWend); }
"print" { TOKEN(KWprint); }
"read" { TOKEN(KWread); }
"return" { TOKEN(KWreturn); }

    /* Identifiers */
{identifier} { TOKEN_STRING(id, yytext); }

    /* Integer Constants */
{dec_num} { TOKEN_STRING(integer, yytext); }
{oct_num} { TOKEN_STRING(oct_integer, yytext); }

    /* Floating-Point Constants */
{float_num} { TOKEN_STRING(float, yytext); }

    /* Scientific Notations */
{scientific_num} { TOKEN_STRING(scientific, yytext); }

    /* String Constants */
{string_const} { 
    char str[MAX_LINE_LENG];
    unsigned int s = 0, y;
    for (y = 1; y < yyleng - 1; ++y) {
        str[s++] = yytext[y];
        if (yytext[y] == '\"' && yytext[y+1] == '\"') ++y;
    }
    str[s] = '\0';
    TOKEN_STRING(string, str); 
}

    /* Whitespace */
" "|\t { LIST; }

    /* Comments */
"//" { BEGIN(CPP_COMMENT); LIST; }

"/*" { BEGIN(C_COMMENT); LIST; }
<C_COMMENT>"*/" { BEGIN(INITIAL); LIST; }

<C_COMMENT,CPP_COMMENT>. { LIST; }

<CPP_COMMENT>\n {
    BEGIN(INITIAL);
    if (opt_src) {
        printf("%d: %s\n", line_num, buffer);
    }
    ++line_num;
    buffer[0] = '\0';
    buffer_ptr = buffer;
}

<INITIAL,C_COMMENT>\n {
    if (opt_src) {
        printf("%d: %s\n", line_num, buffer);
    }
    ++line_num;
    buffer[0] = '\0';
    buffer_ptr = buffer;
}

    /* Pseudocomments */
{pseudocomments} {
    switch (yytext[3]) {
        case 'S': {
            if (yytext[4] == '+') opt_src = 1;
            else if (yytext[4] == '-') opt_src = 0;
            break;
        }
        case 'T': {
            if (yytext[4] == '+') opt_tok = 1;
            else if (yytext[4] == '-') opt_tok = 0;
            break;
        }
    }
    LIST;
}

    /* Catch the character which is not accepted by rules above */
. {
    printf("Error at line %d: bad character \"%s\"\n", line_num, yytext);
    exit(-1);
}

%%

static void concatenateString(const char *yytext_ptr) {
    while (*yytext_ptr) {
        *buffer_ptr = *yytext_ptr;
        ++buffer_ptr;
        ++yytext_ptr;
    }
    *buffer_ptr = '\0';
}

int main(int argc, const char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./scanner [filename]\n");
        exit(-1);
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
        fprintf(stderr, "Open file error\n");
        exit(-1);
    }

    yylex();

    return 0;
}

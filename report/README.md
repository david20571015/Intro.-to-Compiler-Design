# hw1 report

| Field | Value   |
| ----: | :------ |
|  Name | 邱俊耀  |
|    ID | 0710734 |

## How much time did you spend on this project

About 4 hours.

## Project overview

This is a lexical analyzer for the [`P` language](https://github.com/compiler-f21/hw1-david20571015#lexical-definition) implemented by `flex` .

### Usage

- Get docker image: `make docker-pull`
- Activate docker environment: `./activate_docker.sh`
- Build: `make clean && make`
- Execute: `./src/scanner <input file>`
- Test: `make test`

### Implementation

#### Delimiters [Spec](https://github.com/compiler-f21/hw1-david20571015#delimiters)

```lex
%%
"," { TOKEN_CHAR(','); }
";" { TOKEN_CHAR(';'); }
":" { TOKEN_CHAR(':'); }
"(" { TOKEN_CHAR('('); }
")" { TOKEN_CHAR(')'); }
"[" { TOKEN_CHAR('['); }
"]" { TOKEN_CHAR(']'); }
```

#### Arithmetic, Relational, and Logical Operators [Spec](https://github.com/compiler-f21/hw1-david20571015#arithmetic-relational-and-logical-operators)

```lex
%%
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
```

#### Reserved Words [Spec](https://github.com/compiler-f21/hw1-david20571015#reserved-words)

```lex
%%
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
```

#### Identifiers [Spec](https://github.com/compiler-f21/hw1-david20571015#identifiers)

```lex
identifier {letter}({letter}|{digit})*
%%
{identifier} { TOKEN_STRING(id, yytext); }
```

#### Integer Constants [Spec](https://github.com/compiler-f21/hw1-david20571015#integer-constants)

```lex
digit [0-9]
dec_num 0|([1-9]{digit}*)
oct_num 0[0-7]*
%%
{dec_num} { TOKEN_STRING(integer, yytext); }
{oct_num} { TOKEN_STRING(oct_integer, yytext); }
```

#### Floating-Point Constants [Spec](https://github.com/compiler-f21/hw1-david20571015#floating-point-constants)

```lex
digit [0-9]
dec_num 0|([1-9]{digit}*)
float_num {dec_num}\.(0|{digit}*[1-9])
%%
{float_num} { TOKEN_STRING(float, yytext); }
```

#### Scientific Notations [Spec](https://github.com/compiler-f21/hw1-david20571015#scientific-notations)

```lex
digit [0-9]
dec_num 0|([1-9]{digit}*)
nonzero_dec [1-9]{digit}*
nonzero_float ({nonzero_dec}\.(0|{digit}*[1-9]))|({dec_num}\.({digit}*[1-9]))
scientific_num ({nonzero_dec}|{nonzero_float})[eE][+-]?{dec_num}
%%
{scientific_num} { TOKEN_STRING(scientific, yytext); }
```

#### String Constants [Spec](https://github.com/compiler-f21/hw1-david20571015#string-constants)

Truncate leading and trailing `"` and replace `""` by `"`.

```lex
string_const \"([^\"]|\"{2})*\"
%%
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
```

#### Whitespace [Spec](https://github.com/compiler-f21/hw1-david20571015#whitespace)

```lex
%%
" "|\t { LIST; }
```

#### Comments [Spec](https://github.com/compiler-f21/hw1-david20571015#comments)

- C style comments start from `/*` and end in `*/`.
- C++ style comments start from `//` and end in `\n` (newline).

```lex
%x C_COMMENT
%x CPP_COMMENT
%%
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

<C_COMMENT>\n {
    if (opt_src) {
        printf("%d: %s\n", line_num, buffer);
    }
    ++line_num;
    buffer[0] = '\0';
    buffer_ptr = buffer;
}
```

#### Pseudocomments [Spec](https://github.com/compiler-f21/hw1-david20571015#pseudocomments)

```lex
pseudocomments "//&"[A-Z][\+\-].*
%%
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
```

#### Error Handling [Spec](https://github.com/compiler-f21/hw1-david20571015#error-handling)

```lex
%%
. {
    printf("Error at line %d: bad character \"%s\"\n", line_num, yytext);
    exit(-1);
}
```

## What is the hardest you think in this project

- Getting familiar with the syntax of regex and lex.

## Feedback to T.A.s

> Please help us improve our assignment, thanks.

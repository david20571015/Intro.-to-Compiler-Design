%{
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

extern int32_t line_num;  /* declared in scanner.l */
extern char buffer[];     /* declared in scanner.l */
extern FILE *yyin;        /* declared by lex */
extern char *yytext;      /* declared by lex */

extern int yylex(void); 
static void yyerror(const char *msg);
%}

%token ',' ';' ':' '(' ')' '[' ']'
%token ASSIGN VAR
%token ARRAY OF BOOLEAN INTEGER REAL STRING
%token TRUE FALSE
%token DEF RETURN BEGIN_ END WHILE DO IF THEN ELSE FOR TO
%token PRINT READ

%token ID
%token DEC_INT OCT_INT FLOAT SCIENTIFIC STR

%left '*'
%left '/' '%'
%left '+'
%left '-'
%left LT LE NE GE GT EQ
%left AND OR NOT

%%

program
    : ID ';' data_declaration_list function_declaration_or_definition_list compound END
    ;

function
    : ID '(' formal_arguments_list ')' 
    | ID '(' formal_arguments_list ')' ':' scalar_type
    ;

function_declaration
    : function ';'
    ;

function_definition
    : function compound END
    ;

function_declaration_or_definition
    : function_declaration
    | function_definition
    ;

function_declaration_or_definition_list
    :  
    | function_declaration_or_definition_list function_declaration_or_definition
    ;

formal_arguments
    : identifier_list ':' type
    ;

formal_arguments_list
    : 
    | formal_arguments_list_
    ;
formal_arguments_list_
    : formal_arguments 
    | formal_arguments_list_ ',' formal_arguments
    ;

identifier_list
    : ID 
    | identifier_list ',' ID
    ;

data_declaration
    : VAR identifier_list ':' type ';'
    | VAR identifier_list ':' literal_constant ';'
    ;

data_declaration_list
    : 
    | data_declaration_list data_declaration
    ;

type
    : scalar_type
    | ARRAY integer_constant OF type
    ;

scalar_type
    : INTEGER 
    | REAL 
    | STRING 
    | BOOLEAN
    ;

bool_type
    : TRUE 
    | FALSE
    ;

literal_constant
    : integer_constant 
    | '-' integer_constant
    | real_constant 
    | '-' real_constant
    | STR 
    | bool_type
    ;

integer_constant
    : DEC_INT 
    | OCT_INT
    ;

real_constant
    : FLOAT 
    | SCIENTIFIC
    ;

stat
    : compound 
    | simple 
    | conditional 
    | while 
    | for 
    | return 
    | function_call
    ;

stat_list
    :  
    | stat_list stat
    ;

compound
    : BEGIN_ data_declaration_list stat_list END
    ;

simple
    : variable_reference ASSIGN expr ';'
    | PRINT expr ';'
    | READ variable_reference ';'
    ;

variable_reference
    : ID
    | ID array_index
    ;

array_index
    : '[' expr ']' 
    | array_index '[' expr ']'
    ;

conditional
    : IF expr THEN compound END IF
    | IF expr THEN compound ELSE compound END IF
    ;

while
    : WHILE expr DO compound END DO
    ;

for
    : FOR ID ASSIGN integer_constant TO integer_constant DO compound END DO
    ;

return
    : RETURN expr ';'
    ;

function_call
    : ID '(' expr_list ')'  ';'
    ;

expr
    : literal_constant
    | variable_reference
    | ID '(' expr_list ')' 
    | '(' expr ')'
    | '-' expr %prec '*'
    | expr '*' expr
    | expr '/' expr
    | expr '%' expr
    | expr '+' expr
    | expr '-' expr
    | expr relational expr
    | expr AND expr
    | expr OR expr
    | NOT expr
    ;

expr_list
    :
    | expr_list_
    ;
expr_list_
    : expr 
    | expr_list_ ',' expr
    ;

relational
    : LT 
    | LE 
    | NE 
    | GE 
    | GT 
    | EQ
    ;

%%

void yyerror(const char *msg) {
    fprintf(stderr,
            "\n"
            "|-----------------------------------------------------------------"
            "---------\n"
            "| Error found in Line #%d: %s\n"
            "|\n"
            "| Unmatched token: %s\n"
            "|-----------------------------------------------------------------"
            "---------\n",
            line_num, buffer, yytext);
    exit(-1);
}

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./parser <filename>\n");
        exit(-1);
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
        perror("fopen() failed:");
    }

    yyparse();

    printf("\n"
           "|--------------------------------|\n"
           "|  There is no syntactic error!  |\n"
           "|--------------------------------|\n");
    return 0;
}

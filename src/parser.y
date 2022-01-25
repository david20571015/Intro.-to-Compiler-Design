%{
#include "AST/ast.hpp"
#include "AST/program.hpp"
#include "AST/decl.hpp"
#include "AST/variable.hpp"
#include "AST/ConstantValue.hpp"
#include "AST/function.hpp"
#include "AST/CompoundStatement.hpp"
#include "AST/print.hpp"
#include "AST/expression.hpp"
#include "AST/BinaryOperator.hpp"
#include "AST/UnaryOperator.hpp"
#include "AST/FunctionInvocation.hpp"
#include "AST/VariableReference.hpp"
#include "AST/assignment.hpp"
#include "AST/read.hpp"
#include "AST/if.hpp"
#include "AST/while.hpp"
#include "AST/for.hpp"
#include "AST/return.hpp"

#include "AST/AstDumper.hpp"

#include <cassert>
#include <errno.h>
#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <cstring>

#define YYLTYPE yyltype

typedef struct YYLTYPE {
    uint32_t first_line;
    uint32_t first_column;
    uint32_t last_line;
    uint32_t last_column;
} yyltype;

extern int32_t line_num;  /* declared in scanner.l */
extern char buffer[];     /* declared in scanner.l */
extern FILE *yyin;        /* declared by lex */
extern char *yytext;      /* declared by lex */

static AstNode *root;

extern "C" int yylex(void);
static void yyerror(const char *msg);
extern int yylex_destroy(void);
%}

%code requires {
    #include "AST/ast.hpp"
    #include "AST/AstType.hpp"
    #include "AST/ast.hpp"
    #include "AST/program.hpp"
    #include "AST/decl.hpp"
    #include "AST/variable.hpp"
    #include "AST/ConstantValue.hpp"
    #include "AST/function.hpp"
    #include "AST/CompoundStatement.hpp"
    #include "AST/print.hpp"
    #include "AST/expression.hpp"
    #include "AST/BinaryOperator.hpp"
    #include "AST/UnaryOperator.hpp"
    #include "AST/FunctionInvocation.hpp"
    #include "AST/VariableReference.hpp"
    #include "AST/assignment.hpp"
    #include "AST/read.hpp"
    #include "AST/if.hpp"
    #include "AST/while.hpp"
    #include "AST/for.hpp"
    #include "AST/return.hpp"
}

    /* For yylval */
%union {
    /* basic semantic value */
    char *identifier;
    int int_value;
    float real_value;
    bool bool_value;
    const char* string_value;

    enum ScalarDataType scalar_data_type;
    std::vector<int> *dims;
    DataType *data_type;

    AstNode *node;
    std::vector<AstNode*> *node_list;
    
    ConstantValueNode *constant_value_node;
    std::vector<VariableNode*> *variable_node_list;
    DeclNode *decl_node;
    std::vector<DeclNode *> *decl_node_list;
    FunctionNode *function_node;
    std::vector<FunctionNode *> *function_node_list;
    CompoundStatementNode *compound_statement_node;
    ExpressionNode *expression_node;
    std::vector<ExpressionNode *> *expression_node_list;
    FunctionInvocationNode *function_invocation_node;
    VariableReferenceNode *variable_reference_node;
    IfNode *if_node;
    WhileNode *while_node;
    ForNode *for_node;
    ReturnNode *return_node;
};

%type <identifier> ProgramName FunctionName
%type <bool_value> NegOrNot
%type <scalar_data_type> ScalarType
%type <dims> ArrDecl
%type <data_type> Type ArrType ReturnType
%type <node> Statement Simple FunctionCall
%type <node_list> StatementList Statements
%type <constant_value_node> LiteralConstant StringAndBoolean IntegerAndReal
%type <variable_node_list> IdList
%type <decl_node> Declaration FormalArg
%type <decl_node_list> Declarations DeclarationList FormalArgs FormalArgList
%type <function_node> Function FunctionDeclaration FunctionDefinition
%type <function_node_list> Functions FunctionList
%type <compound_statement_node> CompoundStatement ElseOrNot
%type <expression_node> Expression
%type <expression_node_list> Expressions ExpressionList ArrRefs ArrRefList
%type <function_invocation_node> FunctionInvocation
%type <variable_reference_node> VariableReference
%type <if_node> Condition
%type <while_node> While
%type <for_node> For
%type <return_node> Return

    /* Follow the order in scanner.l */

    /* Delimiter */
%token COMMA SEMICOLON COLON
%token L_PARENTHESIS R_PARENTHESIS
%token L_BRACKET R_BRACKET

    /* Operator */
%token ASSIGN
%left OR
%left AND
%right NOT
%left LESS LESS_OR_EQUAL EQUAL GREATER GREATER_OR_EQUAL NOT_EQUAL
%left PLUS MINUS
%left MULTIPLY DIVIDE MOD
%right UNARY_MINUS

    /* Keyword */
%token ARRAY BOOLEAN INTEGER REAL STRING
%token END BEGIN_ /* Use BEGIN_ since BEGIN is a keyword in lex */
%token DO ELSE FOR IF THEN WHILE
%token DEF OF TO RETURN VAR
%token <bool_value> FALSE TRUE
%token PRINT READ

    /* Identifier */
%token <identifier> ID

    /* Literal */
%token <int_value> INT_LITERAL
%token <real_value> REAL_LITERAL
%token <string_value> STRING_LITERAL

%%

Program:
    ProgramName SEMICOLON
    /* ProgramBody */
    DeclarationList FunctionList CompoundStatement
    /* End of ProgramBody */
    END {
        root = new ProgramNode(@1.first_line, @1.first_column,
                               $1, $3, $4, $5);

        free($1);
    }
;

ProgramName:
    ID
;

DeclarationList:
    Epsilon {
        $$ = new std::vector<DeclNode*>();
    }
    |
    Declarations {
        $$ = $1;
    }
;

Declarations:
    Declaration {
        $$ = new std::vector<DeclNode*>();
        $$->push_back($1);
    }
    |
    Declarations Declaration {
        $$ = $1;
        $$->push_back($2);
    }
;

FunctionList:
    Epsilon {
        $$ = new std::vector<FunctionNode*>();
    }
    |
    Functions {
        $$ = $1;
    }
;

Functions:
    Function {
        $$ = new std::vector<FunctionNode*>();
        $$->push_back($1);
    }
    |
    Functions Function {
        $$ = $1;
        $$->push_back($2);
    }
;

Function:
    FunctionDeclaration {
        $$ = $1;
    }
    |
    FunctionDefinition {
        $$ = $1;
    }
;

FunctionDeclaration:
    FunctionName L_PARENTHESIS FormalArgList R_PARENTHESIS ReturnType SEMICOLON {
        $$ = new FunctionNode(@1.first_line, @1.first_column,
                              $1, $3, $5);
    }
;

FunctionDefinition:
    FunctionName L_PARENTHESIS FormalArgList R_PARENTHESIS ReturnType
    CompoundStatement
    END {
        $$ = new FunctionNode(@1.first_line, @1.first_column,
                              $1, $3, $5, $6);
    }
;

FunctionName:
    ID
;

FormalArgList:
    Epsilon {
        $$ = new std::vector<DeclNode*>();
    }
    |
    FormalArgs {
        $$ = $1;
    }
;

FormalArgs:
    FormalArg {
        $$ = new std::vector<DeclNode*>();
        $$->push_back($1);
    }
    |
    FormalArgs SEMICOLON FormalArg {
        $$ = $1;
        $$->push_back($3);
    }
;

FormalArg:
    IdList COLON Type {
        for (auto& var: *$1) {
            var->setType($3);
        }
        $$ = new DeclNode(@1.first_line, @1.first_column, $1);
    }
;

IdList:
    ID {
        $$ = new std::vector<VariableNode *>();
        $$->push_back(new VariableNode(@1.first_line, @1.first_column, $1));
    }
    |
    IdList COMMA ID {
        $$ = $1;
        $$->push_back(new VariableNode(@3.first_line, @3.first_column, $3));
    }
;

ReturnType:
    COLON ScalarType {
        $$ = new DataType($2);
    }
    |
    Epsilon {
        $$ = new DataType(ScalarDataType::VOID);
    }
;

    /*
       Data Types and Declarations
                                   */

Declaration:
    VAR IdList COLON Type SEMICOLON {
        for (auto& var: *$2) {
            var->setType($4);
        }
        $$ = new DeclNode(@1.first_line, @1.first_column, $2);
    }
    |
    VAR IdList COLON LiteralConstant SEMICOLON {
        for (auto& var: *$2) {
            var->setConstant($4);
        }
        $$ = new DeclNode(@1.first_line, @1.first_column, $2);
    }
;

Type:
    ScalarType {
        $$ = new DataType($1);
    }
    |
    ArrType {
        $$ =  $1;
    }
;

ScalarType:
    INTEGER {
        $$ = ScalarDataType::INT_TYPE;
    }
    |
    REAL {
        $$ = ScalarDataType::REAL_TYPE;
    }
    |
    STRING {
        $$ = ScalarDataType::STRING_TYPE;
    }
    |
    BOOLEAN {
        $$ = ScalarDataType::BOOL_TYPE;
    }
;

ArrType:
    ArrDecl ScalarType {
        $$ = new DataType($2, $1);
    }
;

ArrDecl:
    ARRAY INT_LITERAL OF {
        $$ = new std::vector<int>();
        $$->push_back($2);
    }
    |
    ArrDecl ARRAY INT_LITERAL OF {
        $$ = $1;
        $$->push_back($3);
    }
;

LiteralConstant:
    NegOrNot INT_LITERAL {
        if ($1) {
            $$ = new ConstantValueNode(@1.first_line, @1.first_column, -$2);
        } else {
            $$ = new ConstantValueNode(@2.first_line, @2.first_column, $2);
        }
    }
    |
    NegOrNot REAL_LITERAL {
        if ($1) {
            $$ = new ConstantValueNode(@1.first_line, @1.first_column, -$2);
        } else {
            $$ = new ConstantValueNode(@2.first_line, @2.first_column, $2);
        }    
    }
    |
    StringAndBoolean {
        $$ = $1;
    }
;

NegOrNot:
    Epsilon {
        $$ = false;
    }
    |
    MINUS %prec UNARY_MINUS {
        $$ = true;
    }
;

StringAndBoolean:
    STRING_LITERAL {
        $$ = new ConstantValueNode(@1.first_line, @1.first_column, $1);
    }
    |
    TRUE {
        $$ = new ConstantValueNode(@1.first_line, @1.first_column, $1);
    }
    |
    FALSE {
        $$ = new ConstantValueNode(@1.first_line, @1.first_column, $1);
    }
;

IntegerAndReal:
    INT_LITERAL {
        $$ = new ConstantValueNode(@1.first_line, @1.first_column, $1);
    }
    |
    REAL_LITERAL {
        $$ = new ConstantValueNode(@1.first_line, @1.first_column, $1);
    }
;

    /*
       Statements
                  */

Statement:
    CompoundStatement {
        $$ = $1;
    }
    |
    Simple {
        $$ = $1;
    }
    |
    Condition {
        $$ = $1;
    }
    |
    While {
        $$ = $1;
    }
    |
    For {
        $$ = $1;
    }
    |
    Return {
        $$ = $1;
    }
    |
    FunctionCall {
        $$ = $1;
    }
;

CompoundStatement:
    BEGIN_
    DeclarationList
    StatementList
    END {
        $$ = new CompoundStatementNode(@1.first_line, @1.first_column, $2, $3);
    }
;

Simple:
    VariableReference ASSIGN Expression SEMICOLON {
        $$ = new AssignmentNode(@2.first_line, @2.first_column, $1, $3);
    }
    |
    PRINT Expression SEMICOLON {
        $$ = new PrintNode(@1.first_line, @1.first_column, $2);
    }
    |
    READ VariableReference SEMICOLON {
        $$ = new ReadNode(@1.first_line, @1.first_column, $2);
    }
;

VariableReference:
    ID ArrRefList {
        $$ = new VariableReferenceNode(@1.first_line, @1.first_column, $1, $2);
    }
;

ArrRefList:
    Epsilon {
        $$ = new std::vector<ExpressionNode*>();
    }
    |
    ArrRefs {
        $$ = $1;
    }
;

ArrRefs:
    L_BRACKET Expression R_BRACKET {
        $$ = new std::vector<ExpressionNode*>();
        $$->push_back($2);
    }
    |
    ArrRefs L_BRACKET Expression R_BRACKET {
        $$ = $1;
        $$->push_back($3);
    }
;

Condition:
    IF Expression THEN
    CompoundStatement
    ElseOrNot
    END IF {
        $$ = new IfNode(@1.first_line, @1.first_column, $2, $4, $5);
    }
;

ElseOrNot:
    ELSE
    CompoundStatement {
        $$ = $2;
    }
    |
    Epsilon {
        $$ = nullptr;
    }
;

While:
    WHILE Expression DO
    CompoundStatement
    END DO {
        $$ = new WhileNode(@1.first_line, @1.first_column, $2, $4);
    }
;

For:
    FOR ID ASSIGN INT_LITERAL TO INT_LITERAL DO
    CompoundStatement
    END DO {
        VariableNode* var = new VariableNode(@2.first_line, @2.first_column, $2);
        var->setType(new DataType(ScalarDataType::INT_TYPE));

        DeclNode* decl = new DeclNode(@2.first_line, @2.first_column,
                                      new std::vector<VariableNode*>{var});

        AssignmentNode *init = new AssignmentNode(
            @3.first_line, @3.first_column, 
            new VariableReferenceNode(@2.first_line, @2.first_column, $2),
            new ConstantValueNode(@4.first_line, @4.first_column, $4)
        );

        $$ = new ForNode(@1.first_line, @1.first_column,
                         decl, init,
                         new ConstantValueNode(@6.first_line, @6.first_column, $6),
                         $8);
    }
;

Return:
    RETURN Expression SEMICOLON {
        $$ = new ReturnNode(@1.first_line, @1.first_column, $2);
    }
;

FunctionCall:
    FunctionInvocation SEMICOLON {
        $$ = $1;
    }
;

FunctionInvocation:
    ID L_PARENTHESIS ExpressionList R_PARENTHESIS {
        $$ = new FunctionInvocationNode(@1.first_line, @1.first_column, $1, $3);
    }
;

ExpressionList:
    Epsilon {
        $$ = new std::vector<ExpressionNode *>();
    }
    |
    Expressions {
        $$ = $1;
    }
;

Expressions:
    Expression {
        $$ = new std::vector<ExpressionNode *>();
        $$->push_back($1);
    }
    |
    Expressions COMMA Expression {
        $$ = $1;
        $$->push_back($3);
    }
;

StatementList:
    Epsilon {
        $$ = new std::vector<AstNode *>();
    }
    |
    Statements {
        $$ = $1;
    }
;

Statements:
    Statement {
        $$ = new std::vector<AstNode *>();
        $$->push_back($1);
    }
    |
    Statements Statement {
        $$ = $1;
        $$->push_back($2);
    }
;

Expression:
    L_PARENTHESIS Expression R_PARENTHESIS {
        $$ = $2;
    }
    |
    MINUS Expression %prec UNARY_MINUS {
        $$ = new UnaryOperatorNode(@1.first_line, @1.first_column,
                                   new Operator(OperatorType::UNARY_MINUS_OP), $2);
    }
    |
    Expression MULTIPLY Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column,
                                    $1, new Operator(OperatorType::MULTIPLY_OP), $3);
    }
    |
    Expression DIVIDE Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column,
                                    $1, new Operator(OperatorType::DIVIDE_OP), $3);
    }
    |
    Expression MOD Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column,
                                    $1, new Operator(OperatorType::MOD_OP), $3);
    }
    |
    Expression PLUS Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column,
                                    $1, new Operator(OperatorType::PLUS_OP), $3);
    }
    |
    Expression MINUS Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column,
                                    $1, new Operator(OperatorType::MINUS_OP), $3);
    }
    |
    Expression LESS Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column,
                                    $1, new Operator(OperatorType::LESS_OP), $3);
    }
    |
    Expression LESS_OR_EQUAL Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column,
                                    $1, new Operator(OperatorType::LESS_OR_EQUAL_OP), $3);
    }
    |
    Expression GREATER Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column,
                                    $1, new Operator(OperatorType::GREATER_OP), $3);
    }
    |
    Expression GREATER_OR_EQUAL Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column,
                                    $1, new Operator(OperatorType::GREATER_OR_EQUAL_OP), $3);
    }
    |
    Expression EQUAL Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column,
                                    $1, new Operator(OperatorType::EQUAL_OP), $3);
    }
    |
    Expression NOT_EQUAL Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column,
                                    $1, new Operator(OperatorType::NOT_EQUAL_OP), $3);
    }
    |
    NOT Expression {
        $$ = new UnaryOperatorNode(@1.first_line, @1.first_column,
                                   new Operator(OperatorType::NOT_OP), $2);
    }
    |
    Expression AND Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column,
                                    $1, new Operator(OperatorType::AND_OP), $3);
    }
    |
    Expression OR Expression {
        $$ = new BinaryOperatorNode(@2.first_line, @2.first_column,
                                    $1, new Operator(OperatorType::OR_OP), $3);
    }
    |
    IntegerAndReal {
        $$ = $1;
    }
    |
    StringAndBoolean {
        $$ = $1;
    }
    |
    VariableReference {
        $$ = $1;
    }
    |
    FunctionInvocation {
        $$ = $1;
    }
;

    /*
       misc
            */
Epsilon:
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
    if (argc < 2) {
        fprintf(stderr, "Usage: ./parser <filename> [--dump-ast]\n");
        exit(-1);
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
        perror("fopen() failed:");
    }

    yyparse();

    if (argc >= 3 && strcmp(argv[2], "--dump-ast") == 0) {
        AstDumper dumper;
        root->accept(dumper);
    }

    printf("\n"
           "|--------------------------------|\n"
           "|  There is no syntactic error!  |\n"
           "|--------------------------------|\n");

    delete root;
    fclose(yyin);
    yylex_destroy();
    return 0;
}

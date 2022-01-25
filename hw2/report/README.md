# hw2 report

| Field | Value   |
| ----: | :------ |
|  Name | 邱俊耀  |
|    ID | 0710734 |

## How much time did you spend on this project

About 3 hours.

## Project overview

This is a lexical analyzer for the `P` language implemented by `bison`.

### Usage

- Get docker image: `make docker-pull`
- Activate docker environment: `./activate_docker.sh`
- Build: `make clean && make`
- Execute: `./src/parser <input file>`
- Test: `make test`

### scanner.l

先在scanner.l中定義好要return的token，我把Delimiter和一些基本的Operator用符號本身表示，

```yacc
"," { TOKEN_CHAR(','); return ','; }
```

其他則是用單字表示。

```yacc
":=" { TOKEN(:=); return ASSIGN; }
```

另外有一點需要注意，由於`BEGIN`是lex中用來切換start condition的保留字，所以我把`"begin"`的return改成`BEGIN_`。

```yacc
"begin" { TOKEN(KWbegin); return BEGIN_; }
```

### parser.y

為了避免程式寫得太亂，我先依照spec的順序打，最後再把缺的production補上。
我補上的production大部分都是list的，例如：

- statement list: zero or more statements

```lex
stat_list
    :  
    | stat_list stat
    ;
```

- formal argument list: zero or more formal arguments saperated by semicolon

```lex
formal_arguments_list
    : 
    | formal_arguments_list_
    ;
formal_arguments_list_
    : formal_arguments 
    | formal_arguments_list_ ',' formal_arguments
    ;
```

- identifier list: one or more identifiers saperated by semicolon

```lex
identifier_list
    : ID 
    | identifier_list ',' ID
    ;
```

## What is the hardest you think in this project

- 由於syntex analyzer要寫的是context free grammar，所以要一次寫完才能處理錯誤的部分，無法像作業一一樣依序處理每種token，慢慢把test做到滿分，因此要先設計程式的架構。

## Feedback to T.A.s

- 建議可以把spec中一些重點以斜體或粗體特別標示出來，例如：

> The `identifier_list` contains at least one identifier.

改成

> The `identifier_list` contains *at least one* identifier.

或是

> The `identifier_list` contains **at least one** identifier.

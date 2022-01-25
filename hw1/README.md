# Project Assignment 1 - Lexical Definition

**Introduction to Compiler Design by Prof. Yi-Ping You**

### Due date: **23:59, October 18, 2021**

In this assignment, you have to write a scanner for the **`P`** language in `lex`. This document provides the lexical definitions of the `P` language. You should follow those lexical definitions to implement the scanner.

The course project has been divided into five incremental parts:

1. Implement a scanner (lexical analyzer) using `lex` (`flex`). (this assignment)
2. Implement a parser (syntax analyzer) using `yacc` (`bison`).
3. Augment the parser with the appropriate semantic actions to generate an **abstract syntax tree** (**AST**).
4. Augment the parser with appropriate semantic actions to perform a semantic analysis against the AST.
5. Augment the parser with appropriate semantic actions to generate RISC-V assembly code against the AST.

As you can see, each assignment is based on the previous one. Make sure the program you write is well-structured.
### The description video of homework:
https://drive.google.com/drive/folders/1bxhnd03dAB2DbRrAsqgB13DjRHQMJVLW

---

**Table of Contents**
- [Project Assignment 1 - Lexical Definition](#project-assignment-1---lexical-definition)
    - [Due Date:](#due-date)
  - [Character Set](#character-set)
  - [Lexical Definition](#lexical-definition)
    - [Tokens That Will Be Passed to the Parser](#tokens-that-will-be-passed-to-the-parser)
      - [Delimiters](#delimiters)
      - [Arithmetic, Relational, and Logical Operators](#arithmetic-relational-and-logical-operators)
      - [Reserved Words](#reserved-words)
      - [Identifiers](#identifiers)
      - [Integer Constants](#integer-constants)
      - [Floating-Point Constants](#floating-point-constants)
      - [Scientific Notations](#scientific-notations)
      - [String Constants](#string-constants)
    - [Tokens That Will Be Discarded](#tokens-that-will-be-discarded)
      - [Whitespace](#whitespace)
      - [Comments](#comments)
      - [Pseudocomments](#pseudocomments)
  - [Implementation Hints](#implementation-hints)
    - [Token listing](#token-listing)
  - [What Should Your Scanner Do](#what-should-your-scanner-do)
    - [Error Handling](#error-handling)
  - [Project Structure](#project-structure)
  - [Build and Execute](#build-and-execute)
    - [Build project](#build-project)
    - [Test your scanner](#test-your-scanner)
  - [Submitting the Assignment](#submitting-the-assignment)

---

## Character Set

**`P`** programs are formed from ASCII characters. Control characters are not used in the definitions of the `P` language except **`\n`** (line feed) and **`\t`** (horizontal tab).

## Lexical Definition

Tokens are divided into two classes:

1. Tokens that will be passed to the parser.
2. Tokens that will be discarded (i.e., recognized but not passed to the parser).

### Tokens That Will Be Passed to the Parser

The following tokens are recognized by the scanner in this assignment and will be passed to the parser in the next assignment.

#### Delimiters

The following strings are treated as delimiters. Each of them should be recognized by the scanner in this assignment and passed to the parser as a token in the next assignment.

||Delimiter|
|:-:|:-:|
|comma|**`,`**|
|semicolon|**`;`**|
|colon|**`:`**|
|parentheses| **`(`**, **`)`**|
|square brackets| **`[`**, **`]`**|

#### Arithmetic, Relational, and Logical Operators

The following strings are treated as operators. Each of them should be recognized by the scanner in this assignment and passed to the parser as a token in the next assignment.

||Operator|
|:-:|:-:|
|addition|**`+`**|
|subtraction|**`-`**|
|multiplication|**`*`**|
|division| **`/`**, **`mod`**|
|assignment|**`:=`**|
|relational| **`<`**, **`<=`**, **`<>`**, **`>=`**, **`>`**, **`=`** |
|logical|**`and`**, **`or`**, **`not`**|

#### Reserved Words

Each of the following strings is treated as a reserved word of the `P` language. Notice that they are case-sensitive. Each of them should be recognized by the scanner in this assignment and passed to the parser as a token in the next assignment.

- Declaration: `var`, `def`
- Type: `array`, `of`, `boolean`, `integer`, `real`, `string`
- Value: `true`, `false`
Flow-of-control: `while`, `do`, `if`, `then`, `else`, `for`, `to`
- Block: `begin`, `end`
- Statement: `print`, `read`, `return`

#### Identifiers

An identifier is a sequence of letters and digits beginning with a letter. Identifiers are case-sensitive; that is, **gura**, **Gura**, and **GURA** are treated as different identifiers. Note that reserved words CANNOT be used as identifiers.

#### Integer Constants

A sequence of one or more digits. An integer that begins with the digit 0 **and** consists of a sequence of octal digits is treated as an **octal** integer; otherwise, the sequence of digit(s) is treated as a **decimal** integer.

#### Floating-Point Constants

A floating-point constant is formed by an integral part, a dot (`.`), and a fractional part. The dot (`.`) symbol is used to separate the integral part and the fractional part.

The integral part is a decimal integer (see [Integer Constants](#integer-constants)) while the fractional part is a sequence of one or more digits without any redundant `0`. Here are some examples:

- `009.1` is a valid input according to the lexical definition. It is recognized as two separate tokens:
	- An octal integer: `00`
	- A floating-point: `9.1`
- `0.0` is a valid floating-point since there are no redundant `0`s in both side of the dot (`.`) symbol.

#### Scientific Notations

Scientific notation is a way of writing numbers that accommodate very large or small values to be conveniently written in the decimal form. Numbers are written as **`aEb`** or **`aeb`** (**`a`** times ten to the power of **`b`**), where the coefficient **`a`** is a nonzero real number (a nonzero integer or a nonzero floating-point decimal number), and the exponent **`b`** is a **decimal integer** (see [Integer Constants](#integer-constants)) prefixed with an optional sign.

For example: `1.23E4`, `1.23E+4`, `1.23E-4`, `123E4`, etc.

#### String Constants

A string constant is a sequence of zero or more ASCII characters wrapped by two double quotes (`"`). A string constant should not contain any embedded newline(s). A double quote can be placed within a string constant by writing two consecutive double quotes. For example, an input `"aa""bb"` denotes the string constant `aa"bb`.

### Tokens That Will Be Discarded

The following tokens are recognized by the scanner in this assignment, but they are discarded, rather than passed back to the parser, in the next assignment.

#### Whitespace

A sequence of blank spaces, tabs, and newlines.

#### Comments

Comments can be denoted in two ways:

- *C-style* is a text wrapped by **`/*`** and **`*/`**, and may span more than one line. Note that C-style comments **do not "nest"**. Namely, **`/*`** always closes with the first **`*/`** encountered.
- *C++-style* is a one-line text prefixed with **`//`**.

Whichever comment style encountered first remains in effect until the appropriate comment close is encountered. For example, the following two comments are both valid comments:

```
// this is a comment // line  */ /* with some /* delimiters */  before the end

/* this is a comment // line with some /* and	\\
// delimiters */
```

#### Pseudocomments

A psdueocomment is a special form of the *C++-style* comment and is used to signal options to the scanner.

Each **`pseudocomment`** consists of a *C++-style* comment delimiter (`//`), a character **`&`**, an upper-case letter, and either a **`+`** or **`-`** (**`+`** turns the option "on" while **`-`** turns the option "off"). In other words, each **`pseudocomment`** either has the form `//&C+` or the form `//&C-` where `C` denotes the option.

There may be up to 26 different options (A-Z). Specific options will be defined in the project description. A comment that does not match the option pattern exactly has no effect on the option settings. Undefined options have no special meaning; that is, such **`pseudocomments`** are treated as regular comments.

This assignment defines two options, **`S`** and **`T`**. **`S`** turns **source program listing** on or off, and **`T`** turns **token listing** on or off. The option setting takes effect from the line where that pseudocomment is written. By default, both options are on. For example, the following comments are **pseudocomments**:

- `//&S+`
- `//&S-`
- `//&S+&S-`  _**This causes the `S` option on because the comment after the pseudocomment is ignored**_

## Implementation Hints

### Token listing

There are three C macro definitions in the starter code (`src/scanner.l`): **`token`**,　**`tokenChar`**, and **`tokenString`**. You may write your scanner actions using the aforementioned macros or any other ones you define in this manner for implementing the **`token listing`** option.

You may notice that the `#` operator is used in the macro definitions. Let's take the `token(t)` macro for example:

```c
// scanner.l
#define token(t) { LIST; if (opt_tok) printf("<%s>\n", #t); }
```

`#` is a C preprocessor operator, called **Stringizing operator**. Below are some references for you to know more details:

- [**The C Preprocessor** by Alex Allian](https://www.cprogramming.com/tutorial/cpreprocessor.html) (see **Stringizing Tokens**)
- [**Stringizing operator (#)** - C/C++ preprocessor operator for Visual Studio 2019 in MS docs](https://docs.microsoft.com/zh-tw/cpp/preprocessor/stringizing-operator-hash?view=vs-2019)

The meaning and the usage of the macros provided in the starter code are as follows.

- The **`tokenChar`** macro is used for tokens that return a character as well as a token.
- The **`tokenString`** macro is used for tokens that return a string as well as a token.
- The **`token`** macro is used for tokens that are not belonged to the previous two.

The first argument of all three macros is a string. This string names the token that will be passed to the parser in the next assignment. The **`tokenString`** macro takes a second argument that must be a string. Some examples are given below:

|Token|Lexeme|Macro Call|
|:-:|:-:|:-|
|left parenthesis|(|`tokenChar('(');`|
|begin|begin|`token(KWbegin);`|
|identifier|ab123|`tokenString(id,"ab123");`|
|boolean constant|true|`token(KWtrue);`|

## What Should Your Scanner Do

Your scanner must print out source codes and tokens (or not) according to the **`S`**(source program listing), **`T`**(token listing) options.

- If **`S`** (listing option) is on, each line should be listed, along with a line number.
- If **`T`** (token option) is on, each token should be printed on a separate line, wrapped by angle brackets.
    - The **Reserved Words** token should be printed as **`KW<lexeme>`**.
    - The **Identifiers** token should be printed as **`id: <lexeme>`**.
    - The **Integer Constants** token should be printed as **`integer: <lexeme>`** or **`oct_integer: <lexeme>`**.
    - The **Floating-Point Constants** token should be printed as **`float: <lexeme>`**.
    - The **Scientific Notations** token should be printed as **`scientific: <lexeme>`**.
    - The **String Constants** token should be printed as **`string: <lexeme>`**.

For example, given the input:

```pascal
// print hello world
begin
var a : integer;
var b : real;
print "hello world";
a := 1+1;
b := 1.23;
if a > 01 then
b := b*1.23e-1;
//&S-
a := 1;
//&S+
//&T-
a := 2;
//&T+
end if
end
```

Your scanner should output:

```pascal
1: // print hello world
<KWbegin>
2: begin
<KWvar>
<id: a>
<:>
<KWinteger>
<;>
3: var a : integer;
<KWvar>
<id: b>
<:>
<KWreal>
<;>
4: var b : real;
<KWprint>
<string: hello world>
<;>
5: print "hello world";
<id: a>
<:=>
<integer: 1>
<+>
<integer: 1>
<;>
6: a := 1+1;
<id: b>
<:=>
<float: 1.23>
<;>
7: b := 1.23;
<KWif>
<id: a>
<>>
<oct_integer: 01>
<KWthen>
8: if a > 01 then
<id: b>
<:=>
<id: b>
<*>
<scientific: 1.23e-1>
<;>
9: b := b*1.23e-1;
<id: a>
<:=>
<integer: 1>
<;>
12: //&S+
13: //&T-
14: a := 2;
15: //&T+
<KWend>
<KWif>
16: end if
<KWend>
17: end
```

### Error Handling

If the input pattern cannot match any rules, print out the line number and the first character of that input pattern and then abort the program. The output format is as follows.

> `Error at line <line number>: bad character "<character>"`
For example, 
> `Error at line 3: bad character "$"`

## Project Structure

+ README.md
+ /src
  + Makefile
  + **`scanner.l`**
+ /report
  + **`READMD.md`**

In this project, you have to finish `src/scanner.l` and write your report in `report/READMD.md`.
The report should at least describe the abilities of your scanner.

If you want to preview your report in GitHub style markdown before pushing to GitHub, [grip](https://github.com/joeyespo/grip) might be the tool you want.

## Build and Execute

+ Get HW1 docker image: `make docker-pull`
+ Activate docker environment: `./activate_docker.sh`
+ Build: `make`
+ Execute: `./scanner <input file>`
+ Test: `make test`

### Build project

TA will use `src/Makefile` to build your project by simply typing `make clean && make`. Normally, you don't need to modify this file in this assignment, but if you do, it is **your responsibility** to make sure this makefile has at least the same make targets we provided to you.

### Test your scanner

We provide all the test cases in the `test` folder. Simply type `make test` at the root directory of your repository to test your scanner. The grade you got will be shown on the terminal. You can also check `diff.txt` in `test/result` directory to know the `diff` result between the outputs of your scanner and the sample solutions.

Please using `student_` as the prefix of your own tests to prevent TAs from overriding your files. For example: `student_identifier_test`.

## Submitting the Assignment

You should push all your commits to the designated repository (hw1-\<Name of your GitHub account\>) under our GitHub organization by the deadline (given in the very beginning of this assignment description). At any point, you may save your work and push the repository. You **must** commit your final version to the **master branch**, and we will grade the commit which is **last pushed** on your master branch. The **push time** of that commit will be your submission time, so you **should not** push any commits to the master branch after the deadline if you have finished your assignment; otherwise, you will get a late penalty.

Note that the penalty for late homework is **15% per day** (weekends count as 1 day). Late homework will not be accepted after sample codes have been posted.

In addition, homework assignments **must be individual work**. If I detect what I consider to be intentional plagiarism in any assignment, the assignment will receive reduced or, usually, **zero credit**.

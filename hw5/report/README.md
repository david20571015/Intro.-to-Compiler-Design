# hw5 report

| Field | Value   |
| ----: | :------ |
|  Name | 邱俊耀  |
|    ID | 0710734 |

## How much time did you spend on this project

About 12 hours.

## Project overview

This is a RISC-V code generator for the `P` language implemented by `bison`.

### Stack machine

In this project, I used the **stack machine** model to implement the RISC-V code generator.

The simplified workflow of the stack machine is:

1. Pop the arguments from the stack.
2. Compute the result.
3. Push the result to the stack.

### Local Variables and Constants

In order to declare variables in a scope, I used `current_sp`, `current_fp` and `current_offset` to record current status of the stack pointer, frame pointer and the offset of the variables. And set the absolute position of a `entry`.

I initialized `current_sp` and `current_fp` to `0` and `current_offset` to `-12`. While entering a scope, `current_sp` was increased by `-128`, `current_fp` was set to `current_sp + 128`, and `current_offset` was set to `-12`.

In each scope, assume the stack pointer = -128 and the frame pointer = 0, the data should be stored like

```txt
0   -4   -8     -12    -16      -124  -128
-------------------------- ..... -------
| ra | s0 | var1 | var2 |  .....  |    |
-------------------------- ..... -------
```

While declaring a variable, first set the absolute position of the entry as `current_fp + current_offset`, set the value of the variable, then increased `current_offset` by `-4`, which was the position of next variable.

To reference the variable, we could get the absolute position of the variable, then compute the offset of the variable from the current frame pointer by `entry->getPosition() - current_fp`, finally, push the address/value of the variable into the stack.

### Expressions

> *t0 = t1 op t0* for binary operators.

> *t0 = op t0* for unary operators.

#### Arithmetic Operations

* `+`, `-`(binary), `*`, `/`, `mod`

    Simply used `add`, `sub`, `mul`, `div`, `mod` to perform the operation.

    ```assembly
    add t0, t1, t0
    ```
  
* `-`(unary)

    Performed an additive inverse number by subtracting the number from 0.

    ```assembly
    sub t0, zero, t0
    ```

#### Logical Operations

RISC-V provided few logical operations, but we could combine them to perform more logical operations.

> The operands are boolean values(0 or 1).

* `and`, `or`

    Simply used `and`, `or` to perform the operation.

    ```assembly
    and t0, t1, t0
    ```

* `not`

    Used `seqz`(set if equal to zero). If the result is 0, then the result is 1, otherwise, the result is 0.

    ```assembly
    seqz t0, t0
    ```

* `>`, `>=`, `<`, `<=`, `=`, `<>`
    Used `slt`(set if less than) to perform `<`, and combined `sub`, `seqz` and `snez` to perform `=` and `<>`. Then combine both operations to perform other operations.

    ```assembly
    // <
    slt t0, t1, t0
    // <=
    slt t0, t0, t1
    seqz t0, t0
    // >
    slt t0, t0, t1
    // >=
    slt t0, t1, t0
    seqz t0, t0
    // =
    sub t0, t1, t0
    seqz t0, t0
    // <>
    sub t0, t1, t0
    snez t0, t0
    ```

### Branching

We needed to insert a unique label in the assembly code for branching. In the project I used `getAnonymizedLabel` to retrieve the unique label.

```cpp
static std::string getAnonymizedLabel() {
  static int label_count = 0;
  return std::string("L") + std::to_string(label_count++);
}
```

For example, in IfNode, I could retrieve the unique label by `getAnonymizedLabel()`

```cpp
const auto true_label = getAnonymizedLabel();
const auto false_label = getAnonymizedLabel();
const auto end_label = getAnonymizedLabel();
```

Then inserted it like

```cpp
dumpInstructions(m_output_file.get(),
                 "%s:\n",
                 end_label.c_str());
```

## What is the hardest you think in this project

1. How to pass numerous arguments to a function?

    In RISC-V we usually use `a0` to `a7` to pass arguments. But since we want to pass more than eight arguments to a function. We need to use `t3` to `t6` to pass arguments.

    My implementation was declaring an array of registers of arguments:

    ```cpp
    static const char *kArgReg[] = {"a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "t3", "t4", "t5", "t6"};
    ```

    Then I could pass arguments in `FunctionInvocationNode`:

    ```cpp
    for (int i = p_func_invocation.getArguments().size() - 1; i >= 0; i--) {
      dumpInstructions(m_output_file.get(),
                       "    lw %s, 0(sp)\n"
                       "    addi sp, sp, 4\n",
                       kArgReg[i]);
    }
    ```

    And parse arguments in `VariableNode`:

    ```cpp
    if (entry->getKind() ==
        SymbolEntry::KindEnum::kParameterKind) {  // Function Parameters
      entry->setPosition(current_fp + current_offset);
      dumpInstructions(m_output_file.get(),
                       "# Function parameter\n"
                       "    sw %s, %d(s0)\n",
                       kArgReg[(-current_offset - 12) / 4], current_offset);
                       // arg0 was store in offset = -12
                       // arg1 was store in offset = -16
                       // ...
      current_offset += -4;
    }
    ```

## Feedback to T.A.s

This is the last project of this course. I thought it was a good experience to learn how to write assembly code, but it was a little difficult for me to accomplish all the bonus. Hope T.A.s will provide the complete code of this project by any chance.

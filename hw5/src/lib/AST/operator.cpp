const char *kOpString[] = {"neg", "*",  "/", "mod", "+",   "-",   "<", "<=",
                           ">",   ">=", "=", "<>",  "not", "and", "or"};

// Use logically inverse op for conditions.
const char *kOpAssembly[] = {"sub", "mul", "div", "rem", "add",
                             "sub", "bge", "bgt", "ble", "blt",
                             "bne", "beq", "#",   "#",   "#"};

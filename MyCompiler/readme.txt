Compiler v 0.1

This compiler does not yet output to a bytecode but rather to a summary of the compiler's conception of a program from source code to very nearly ready to translate to final byte code. Each stage is output showing what is currently going on.

The program is written in C++ and should work fine in QtCreator.

A couple of notes:

Primitive boolean types, arrays, literals and classes are not fully supported. All of these require little modification, except for array support which may require a bit of a re-think. Unary operators are not supported.

Functions determine scope. Variable declarations are hoisted to the top of their scope, and arguments are pulled down from function headers to their bodies. Control statements (if,else,while, etc..) are flattened, to be replaced with CMP and GOTO-style statements. Lexical Scopes are flattened only after each one is assigned a unique id and a record of parentage established. It is believed this may be necessary to enforce lexical scoping at runtime.

All expressions are converted into reverse polish notation and separated out into expressions of the form left right root, so that each instruction may be of a fixed length.

For future:

Implement support for features noted to be lacking above.

Put in place error detection and validation to handle syntax errors.

Every literal and variable name(stored as strings) to be collected and stored as a byte array to be dumped on the heap as the first instruction.

Every reference to every literal or variable name to be replaced with integers pointing to their expected position in the heap.

Lots of debugging.

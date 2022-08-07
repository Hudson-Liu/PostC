# Table Of Contents
* Will be replaced with the ToC, excluding the "Contents" header
{:toc}

# Introduction

**"++C" is a C++ based esolang** (esoteric programming language) created as a demonstration of using postfix notation in programming syntax. 

The ++C interpreter currently supports only the following:
1. The 3 datatypes int, string, and bool
2. Basic arithmetic operators (addition, subtraction, multiplication, division)
3. Variable declaration and assignment
4. Comments
5. Basic input/output with cout and cin

#### Note
_In order for the ++C interpreter to support more functionalities without becoming large and cumbersome, the source code would need to be overhauled to rely less on C++ constructs and more on actual lower-level logic. The current interpreter represents ++C variables as elements of C++ lists, with each datatype having it's own list. Directly allocating memory and creating pointers would be much more efficient and scalable than using C++ lists, but would require an overhaul of the current interpreter._

# Postfix Notation

++C (or PostC) is based off of a notation called postfix notation.

**Postfix notation** is a mathematical notation where the operator appears after the operands. 
The usual way people write mathematical expressions is called **infix notation**, as the operators are placed directly between the two operands.

The infix expression `4 + 3` can be written in postfix notation as `4 3 +`. 

Postfix notation is oftentimes taught alongside the **stack** abstract data type (ADT). A stack is a type of collection that follows the **LIFO** (Last In First Out) principle. A postfix expression can be imagined as a stack of operands. By reading a postfix expression from left to right, adding each operand to a stack, and having each operator **pop 2 operands** off the stack and **push the result** back onto the stack, any postfix expression can be easily evaluated.

[insert image of stack here]

As such, infix expressions such as `4 + 3 * 2` can be easily expressed in postfix notation as `4 3 2 * +`.

++C uses this concept of operators and operands and extends it to **programming**. Instead of simple operators such as multiplication and division, ++C treats all C++ syntax such as "int", "cout", and "<<" as operators and operands.

Each line of code is represented by a **stack of operands**, and **semicolons** notate the end of a line and the resetting of the stack.

#### Note
_A notable detail about postfix notation is that, unlike infix notation, spaces are required to make an equation work. In infix notation, spaces are not required as the operators separate the two operands. But in infix notation, nothing besides space separates the two operands, and as such, whitespace is an essential part of ++C._

# Rvalue and Lvalue
The **lvalue** and **rvalue** of an operator are shorthands for talking about the left operand and right operand of a **binary operator**.

There are a few notable differences between ++C operands and mathematical operands
- In cases where the return value can be either operand (e.g. does `"hi" cout <<` return `cout` or `"hi"`), the right operand will always be returned
- Some operators, such as `+`, do not require their operands in a specific order, while others, such as `<<`, require specific lvalues and rvalues
- Unary operators exist

#### Note
_Although this isn't the typical use of the terms lvalue and rvalue, they fit this use case pretty well and I don't feel like coming up with new terminology._

# Comments

In ++C, **all comments are multiline comments**. This choice stems from the idea that newlines should not affect the functionality of code, which is why the ++C interpreter automatically **removes all newline characters**. C++ style comments would not make sense without newlines, and therefore all comments are given a distinct start and end.

All comments are started with a "//" and ended with another "//". The following is a valid comment:

```
// Hi! I'm a comment //
```
If a comment splits an identifier in two, the interpreter will ignore the comment and act as if the identifier was still uninterrupted. For instance, despite the comment directly intersecting the "cout", the following code will still run correctly:
```
"test" co// This code is still valid //ut <<
```

# Variable Assignment

A simple integer assignment:
```
x int 0 =;
```
`x int` declares x as an int and returns x to the stack. Then, with `x` and `0` on the stack, the assignment operator `=` removes them from the stack and assigns `x = 0`. The semicolon then resets the stack for the next line.

This works the same way for all datatypes.

# Printing Text: "cout"

Printing "Hello World":
```
"Hello World" cout <<;
```
The **stream insertion operator** `<<` takes in a **string, string variable, or modifier** as it's lvalue and an output stream as it's rvalue. 

This operator returns `cout`, so multiple print statements can be stringed together as such:
```
endl "!" "Hello World Again" cout << << <<;
```
As shown above, `cout` also supports the `endl` modifier. ++C does not support any modifiers other than `endl`.

`cout` can also take string variables as an lvalue:
```
message string "Goodbye World" =;
message cout <<;
```

# User Input: "cin"

Receiving a string and saving it to a variable:
```
input string cin >>;
```
The **stream extraction operator** `>>` takes in any **string, int, or bool variable** as it's lvalue and an input stream as it's rvalue.

Like the stream insertion operator, the **stream extraction operator** can also be strung together with multiple inputs:
```
input2 string input1 string cin >> >>;
```

The string, int, and bool datatypes are all supported by cin, but require a valid input:
```
input3 bool input2 int input1 string cin >> >> >>;
```
If a valid input isn't given, an exception will be thrown.

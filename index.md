## Introduction

"++C" is a C++ based programming/scripting language, created as a demonstration of using postfix notation in programming syntax. 

The ++C interpreter currently supports only the following:
1. The 3 datatypes int, string, and bool
2. Basic arithmetic operators (addition, subtraction, multiplication, division)
3. Variable declaration and assignment
4. Comments
5. Basic input/output with cout and cin

##### Note
_In order for the ++C interpreter to support more functionalities without becoming large and cumbersome, the source code would need to be overhauled to rely less on C++ constructs and more on actual lower-level logic. The current interpreter represents ++C variables as elements of C++ lists, with each datatype having it's own list. Directly allocating memory and creating pointers would be much more efficient and scalable than using C++ lists, but would require an overhaul of the current interpreter._

## Postfix Notation

++C (or PostC) is based off of a notation called postfix notation.

**Postfix notation** is a special mathematical notation where the operator appears after the operands. 
The usual way people write mathematical expressions is called **infix notation**, as the operators are placed directly between the two operands.

The infix expression `4 + 3` can be written in postfix notation as `4 3 +`. 

Postfix notation is oftentimes taught alongside the **stack** abstract data type (ADT). A stack is a type of collection that follows the **LIFO** (Last In First Out) principle. A postfix expression can be imagined as a stack of operands. By reading a postfix expression from left to right, adding each operand to a stack, and having each operator **pop 2 operands** off the stack and **push the result** back onto the stack, any postfix expression can be easily evaluated.

[insert image of stack here]

As such, infix expressions such as `4 + 3 * 2` can be easily expressed in postfix notation as `4 3 2 * +`.

++C uses this concept of operators and operands and extends it to **programming**. Instead of simple operators such as multiplication and division, ++C treats all C++ syntax such as "int", "cout", and "<<" as operators and operands.

Each line of code is represented by a **stack of operands**, and **semicolons** notate the end of a line and the resetting of the stack.


##### Note
_A notable detail about postfix notation is that, unlike infix notation, spaces are required to make an equation work. In infix notation, spaces are not required as the operators separate the two operands. But in infix notation, nothing besides space separates the two operands, and as such, whitespace is an essential part of ++C._

## Terminology


## Comments

In ++C, all comments are multiline comments. One of the ideas behind ++C was that newlines should have no impact on the functionality of code, and as such, all newlines are erased from ++C code upon being loaded by the interpreter. Therefore, the traditional C++ comment syntax would make no sense in ++C.

All comments are started with a "//" and ended with another "//". The following is a valid comment:

```
// Hi! I'm a comment //
```
If a comment splits an identifier in two, the interpreter will ignore the comment and act as if the identifier was still uninterrupted. For instance, despite the comment directly intersecting the "cout", the following code will still run correctly:
```
"test" co// This code is still valid //ut <<
```

## Variable Assignmnet

For more details see [Basic writing and formatting syntax](https://docs.github.com/en/github/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax).

### Jekyll Themes

Your Pages site will use the layout and styles from the Jekyll theme you have selected in your [repository settings](https://github.com/Hudson-Liu/postc.github.io/settings/pages). The name of this theme is saved in the Jekyll `_config.yml` configuration file.

### Support or Contact

Having trouble with Pages? Check out our [documentation](https://docs.github.com/categories/github-pages-basics/) or [contact support](https://support.github.com/contact) and we’ll help you sort it out.

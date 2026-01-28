# Overview

Hex++ is a high-level language that compiles into patterns for Hex Casting, a Minecraft mod. This document will give a basic overview of how writting a program in Hex++ works.

## Documentation

This sections contains a list of all docs and what they contain.

* overview.md: This document, described above.
* statements.md: A list of all statements that are valid code, including if, while, return, etc.
* expressions.md: A list of all operators that are used in expressions, as well as other information involved in evaluating expressions.
* functions.md: Information on declaring functions and a list of all inbuilt functions.
* examples.md: A collection of example programs along with descriptions of what they do.
* grammer.pdf: The basic grammer used in composing a program, describes what is or is not valid syntax.

## Making a program

Hex++ programs start with their main functions. Here's what the definiion of one looks like:
```
void main() {

}
```
Inside the curly braces, differnet statements can be made, all of which are detailed in statements.md. In addition to the main function, any number of void or non-void functions may be declared, like so:
```
void foo(var1, var1) {

}

ret bar() {
    return null;
}
```
The details about functions are listed in functions.md, along with a description of inbuilt functions. These inbuilt functions are going to be how you get your program to actually do stuff (break blocks, make lights, etc.). Programs also may have global variables declared in them:
```
let global1 = 0;
let global2 = 3;
let global3 = global1 + global2;
```
Global variables can reference inbuilt functions and other global variables that were declared higher up than them, but NOT functions you defined or global variables declared lower than them! That's it for the basics, here's an example of what a program may look like all put together:
```
let foo = 1;

void foo_bar(num){
    return num / 2;
}

let bar = foo + 1;

ret bar_foo(num) {
    return num * 2;
}

void main() {
    print (foo_bar(bar) * bar_foo(foo));
}
```
This spell prints out the number 2 into chat. This is simply an example to show you what a program looks like, if you want more practical examples, you can see some in examples.md.

## Comments

At almost any point in a program, you may insert comments like so:
```
void main() {
    // Print 1 into the caster's chat
    print(1);
}
```
Comments are not considered as part of the code, the compiler will ignore them completely. This can allow you to explain what your code is doing, make notes, etc. This comment is on code that is quite self-explanatory, but sometimes they may help clarify code that would otherwise be hard to parse.

There are two kinds of comments, single-line comments as shown above, and multi-line comments. Single-line comments start at a //, and end at the next new line. Multi-line comments start at a /*, and end at */. This allows your comment to span multiple lines, like so:
```
void main() {
    /* This is a long comment,
    so I decided to split it
    up into multiple lines.
    I could also comment like
    this if I had lines of
    code I wanted to comment
    out temporarily. */
    print (1);
}
```
Comments are a helpful tool for making your code easy for yourself and others to understand, as well as for debugging.

## Pattern Usage

For the sake of allowing you to make more efficient spells, every feature of this language will tell you how many patterns it costs to use. A single spell cannot evaluate more than 100,000 patterns. Every program will generate 1 pattern as a base amount, with more being generated depending on what code you write. Some things may not use patterns, and so will not list how many they use, such as variable or function declaration. However, this is only for that bit of the code itself, for example declaring a variable may not require patterns, but generating the expression it's being set to does. The pattern costs listed are the maximum amount that the code could cost, however there are some compile-time optimizations that may reduce that count.

One last thing to note is that in the case of functions or loops, the patterns within them will count towards the 100,000 for each time they are ran. A loop which has 30 patterns worth of code in it will cost 30 patterns per loop it does, however if a function containing 100 patterns is never ran, it won't hurt your pattern count at all.
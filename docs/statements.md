# Statements

Statements are how you tell the program to do things. This includes statements such as return, while, or even function calls. This document contains an exhaustive list of all statements available in Hex++ and what they do. Most statements require a ';' after them, but not all, any which don't will list it.

## Expressions

Many statements reference expressions. These are detailed in expressions.md. They are essentially a collection of terms and operations which are evaluated into a single iota. So they could just be a number, or 2 numbers added, or even just a function call. Something important to note is that the pattern cost listed for each statement does not take into accound the pattern cost of any expressions used in the statement. A variable declaration may not use any patterns, but the expression it's set to might.

## Statement List

1. Function Call
2. Return
3. Expression (Includes variable re-assignment)
4. Variable Declaration
5. If & Else
6. While
7. Scope

## 1. Function Call

### Patterns Used

The patterns used by a function call vary based on the kind of function. For declared functions, the amounts listed are assuming that the end of the function is reached without a return. If a return is called, all declared functions are 4 patterns, plus the patterns executed in the function. This is in counting the patterns used by the return statement, of course.

* Inbuilt: Amount specific to each function  listed in functions.md
* Void Declared: 6, plus the patterns executed in the function
* Non-Void Declared: 8, plus the patterns executed in the function

### Description

Function calls are statements that cause the program to execute some pre-defined code. Technically speaking, only void function calls are statements, non-void function calls are terms, as would typically be explained in expressions.md. However, they are similar in the ways they are used and called, so while details on their use as a term will be described there, calling them will be described here. Additionally, as you will see below, expressions can themselves be statements, meaning that non-void functions can also be called like statements.

There are two kinds of functions, declared ones and inbuilt ones. The details about these are given in functions.md, and inbuilt functions as described there will be how your program gathers information about the world and affects it, so it is important to know what kinds of functions are available for you to use. This includes spells and great spells.

Where that document goes over what a function is and how to define it, this section will go over calling them. Like many languages, Hex++ uses a syntax like this to call a function:
```
func_name(param1, param2, ..., paramN);
```
When called, the function will do as defined by you if it's a declared function, or do as described in functions.md if it's an inbuilt function. The paramaters passed into the functions are each expressions, and whatever they evaluate to will be passed into the function, with each parameter being a local variable in the called function set to the passed expressions' outputs. They are always assigned based on the order passed in, passing parameters in the wrong order will likely result in mishaps when casting the compiled spell, or incorrect behaviour.

If you pass an invalid number of parameters into a function, the compiler will give you an error message saying so.

There are two ways to call functions, as member or non-member functions. To call a non-member function, you use the syntax used in the example above. But to call a member function, you do like so:
```
variable.member_func(param1, param2, ..., paramN)
```
Currently, there is no way for you to define your own member functions in Hex++, all member functions will be inbuilt ones. They are typically used when the variable they're being called on is in some way central to the functions being called, such as the 'with' or 'pos' inbuilt functions.

## 2. Return

Patterns Used: 2 in void functions, 3 in non-void functions

Return statements allow you to cut short a function, including a main function, and return back to where the function was called from. The syntax in Hex++ is like so:
```
return; // Void return
return expr; // Non-void return
```
If called in a void or non-void function, it will return to where the function was called, or end the spell completely if used in main. But, if used in a non-void function, it must also be given an expression. The expression given will be the value returned by the function.

Note: Non-void functions which reach the end without executing a return will automatically return null.

## 3. Expression

Patterns Used: 1, plus however many the expression uses.

Expressions themselves can be statements. For most of your typical expressions, such as 1+2, this won't achieve anything. But some expressions change the values of variables or call functions. For instance,this is an expression:
```
var = expr
```
This is how variable re-assignment is done, it is handeled as an expression. This means that details about variable assignment will be handeled in expressions.md rather than here. Here's a list of notable statement-like expressions which will be covered in that document:
* Variable re-assignment (=, +=, *=, etc.)
* Pre/Post Increment/Decrement
* Non-Void Function Calls

## 4. Variable Declaration

Patterns Used: 0

Variable declaration is how local variables are named and given an initial value. Local variables may then be referenced as terms inside expressions. The syntax for declaring a local variable is like this:
```
let var = expr;
```

## 5. If & Else

Patterns Used: 5, often optimized to 4

If statements allow you to only run code in ceratin conditions. If statements do not require a ';'. The syntax for them in Hex++ looks like this:
```
if (expr) statement
else statement
```
The expression is determines if the first statement is ran or the second statement. If it evaluates to true, as per Hex Casting's Augur's Purification pattern, then the first statement will be ran, otherwise it will run the second. This means the expression may evaluate to anything, not just a bool, and it will automatically be interpreted as a bool.

The second half (starting with else) is optional. If left out, it will run the first (only, in this case) statement if the expression is true, and will run nothing otherwise, continuing on to whatever statements follow the if.

While ifs and elses can only run single statements, they usual run two particular statements that allow for more flexible use. The primary statement they run are scope statements, explained below in this document. When running these, they may run multiple lines of code simultaneously. Additionally, the compiler will optimize away the pattern cost of the scope if used in this way. Using them in this way looks like this:
```
if (expr) {
    statement1
    statement2
    ...
    statementN
}
else {
    statement1
    statement2
    ...
    statementN
}
```
The exact placement of the curly braces is flexible. You can place them on the same line as the if, after, or even several lines after. The else may also be on the same line as the '}' from the if's scope if you prefer.

The second kind of statement typically used is another if statement. This allows for 'else if' statements to be made, like so:
```
if (expr1)
{

} else if (expr2)
{

} else
{

}
```
The program will evaluate the first expression, if true it will run the first scope then skip all the following 'else if' and 'else' statements. If false, it will evaluate the second expression, and it will do the same for the 'else if' statement's scope if true. Finally if that's false, it will run the else's scope. This may be chained any number of times. Note that the pattern cost is for each if, so each 'else if' will increase the pattern cost accordingly.

## 6. While

Patterns Used: 4, plus an additional 3 for each time the loop runs

Similar to if statements, while statements will run the statement given to them if the expression they're given is true. Unlike if statements, they do not have an else, and when statement given to them finishes running, it will check the expression again, and if true, will run the statement again. This continues until the expression evaluates to false. While statements do not require a ';'. The syntax looks like this:
```
while (expr) statement
```
Similar to if statements, you'll likely want to use a scope statement as the statement, so that multiple statements may run inside the loop.

## 7. Scope

Patterns Used: 1

When a scope statement is ran, it will run all statements inside of it. Scope statements do not require a ';'. The syntax in Hex++ looks like this:
```
{
    statement1
    statement2
    ...
    statementN
}
```
Additionally, any variables declared inside the scope will only exist for the duration of the scope, they cannot be referenced by code outside the scope.
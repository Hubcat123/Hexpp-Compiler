# Expressions

Expressions are collections of terms and operators that evaluate into a singles value, or in Hex Casting terms, a single iota. An expression can be as simple as a single number, a function call, or adding two numbers together, to as complex as you want (within the limitations of your computer and Hex Casting).

An expression has two parts as stated above, terms and operators. This document will go over all possible kinds of each in the following sections. In order to evaluate an expression, each term in the expression will have the operators in the expression applied to it if applicable, and it will return a result. Here's an example expression:
```
3 / (4 + 2)
```
As you can probably tell, this expression evaluates to 0.5, and this resulting value can be used somewhere else in your code, for instance telporting the caster that many blocks up, or causing an explosion with that amount of power.

## Terms

Terms are the values that are used in expressions. This could be a number, a boolean value, a non-void function call, and more, as described below. These terms are then evaluated using the operators in the same expression as them, as described in the Operators section. Here is a list of all kinds of terms.

### Term Types

1. Number Literal
2. Boolean Literal
3. Null Literal
4. List
5. Variable
6. Function Call
7. Parenthesis

### 1. Number Literal

Patterns Used: 1

Number literals are hard coded numbers. This can be an integer without any decimal places, or a non-integer with them. Examples include 3, 7, 24, 5.6, etc.

An important note is that Hexagon (if you're using it) cannot handle decimal numbers, it will round them down whenever it is given them. For this reason, it is suggested you use operations to get any hard-coded decimal numbers you want to use, i.e. use 1/2 instead of 0.5, use 56/10 instead of 5.6, etc.

### 2. Boolean Literal

Patterns Used: 1

Boolean literals are hard coded bools, namely true and false. In code they look like this:
```
true
false
```

### 3. Null Literal

Patterns Used: 1

Null literals are hard coded null values. In code that look like this:
```
null
```

### 4. List

Patterns Used: 2, plus any used in exvaluating the given expressions; 0 if the list is empty

Lists are a list of values of any kind, as per Hex Casting. When used as a term, any number of elements may be added, including none. The syntax looks like this:
```
[expr1, expr2, ..., exprN]
```

### 5. Variable

#### Pattern Cost

Base cost if local: 2
Base cost if global: 3

Add if list value: +1, plus patterns for evaluating expression

#### Description

Variables are references to previously declared local variables or global variables. They are used by typing the name of the variable to be used. They can also reference specific values at a list index, like so:
```
var[expr]
```

### 6. Function Call

Patterns Used: Listed in statements.md

Most of the description of function calls is done in statements.md, which explains both void and non-void functions, as they are similar. As a term, the value of the term is equal to the return value of the function, as explained in functions.md. This means that only non-void functions may be a term, the compiler will give an error message if a void function is used like a term.

### 7. Parenthesis

Patterns Used: 0

Parenthesis may be used to encapsulate an expression and make it a term. In practice, this forces operator precedence (explained in the operator section) to "reset". Use looks like this:
```
1 / (2 - 4)
```
Normally, the division would be evaluated first if the parenthesis weren't there, resulting in a value of -3.5. But since the parenthesis are there, the inner part is evaluated first, resulting instead in -0.5.

## Operators

There are two kinds of operators, unary and binary. Unary operators operate on a single term, where as a binary operator operates on a term and an expression. The layout is as shown here:
```
-term
term + expr
```
Some unary operators can also be post operators instead of pre operators, which looks like this:
```
term++
```
Post operators may work differently than the pre operator versions. The following sections will describe each operator.

### Assignment Operator Costs

The pattern cost for operators which assign variables is fairly complex, and so it will be explained in a seperate section here.

There are 3 primary important features in assignment that determine the cost: if we're assigning a list element, if the variable we're setting is global, and if the operator is a pre or post operator. In general, assume the operator is a 'pre' operator for operators which don't say.

Assume base pattern cost of 2, then check for these cases:

* If assigning list element: +6
* If global: +3
* If post: +1
* If post and global: +1 (This applies in addition to the two individual cases above)

As you can see, assigning certain kinds of variables can get very expensive for patterns. Keep this in mind when writing your code.

### Assignment Operator List Return Values

Due to trying to limit the already high pattern costs of assigning to list elements, operators which assign list elements will not return the list element as you may expect. Instead, they return the full list. This does not apply to non-assignment operators.

### Unary Operators

#### Negation

Patterns Used: 2

The negation operator, '-', takes the given term and makes it negative. Works on numbers or vectors.

#### Invert

Patterns Used: 1

The invert operator, '~' or '!', will invert a boolean, turning true to false and visa versa, or will invert each bit of a number.

#### Increment

Pattern Cost: 1, plus as described above for operator pattern costs

The increment operator, '++', only works on variable terms. It will take the passed variable, add one to it, and set the variable to that new value. The value the increment operator returns depends on if it is used as a pre operator or a post operator. If it is a pre operator, it will return the value of the variable plus 1, the same as it was just set to. If it's a post operator, it will return the value of the variable before the 1 was added to it.

#### Decrement

Pattern Cost: 1, plus as described above for operator pattern costs

The increment operator, '--', only works on variable terms. It will take the passed variable, subtract one from it, and set the variable to that new value. The value the increment operator returns depends on if it is used as a pre operator or a post operator. If it is a pre operator, it will return the value of the variable plus 1, the same as it was just set to. If it's a post operator, it will return the value of the variable before the 1 was added to it.

### Binary Operators

#### Operator Precedence

Binary operators have varying precedence levels. Those with higher precedence values will evaluate first. For instance, in the following expression:
```
1 + 2 / 3
```
The division will evaluate first since it has higher precedence than the addition. This is allows PEMDAS/BODMAS to be followed. Here is a list of the precedences of each operator:

* +=;  prec = 0
* -=;  prec = 0
* *=;  prec = 0
* /=;  prec = 0
* %=;  prec = 0
* =;   prec = 0
* ||;  prec = 1
* &&;  prec = 2
* ^;   prec = 3
* ==;  prec = 4
* !=;  prec = 4
* \>=; prec = 5
* <=;  prec = 5
* \>;  prec = 5
* <;   prec = 5
* +;   prec = 6
* -;   prec = 6
* *;   prec = 7
* /;   prec = 7
* %;   prec = 7

#### Plus Assignment

Patterns Used: As described above for operator pattern costs

The plus assignment operator, '+=', takes the value of the expression on the right, adds it to the value of the variable on the left, and sets the variable on the left to that value. It returns the new value of the variable. The left term must be a variable.

#### Minus Assignment

Patterns Used: As described above for operator pattern costs

The minus assignment operator, '*=', takes the value of the expression on the right, subtracts it from the value of the variable on the left, and sets the variable on the left to that value. It returns the new value of the variable. The left term must be a variable.

#### Multiply Assignment

Patterns Used: As described above for operator pattern costs

The multiply assignment operator, '*=', takes the value of the expression on the right, multiplies it by the value of the variable on the left, and sets the variable on the left to that value. It returns the new value of the variable. The left term must be a variable.

#### Divide Assignment

Patterns Used: As described above for operator pattern costs

The divide assignment operator, '/=', takes the value of the expression on the right, divides it by the value of the variable on the left, and sets the variable on the left to that value. It returns the new value of the variable. The left term must be a variable.

#### Modulus Assignment

Patterns Used: As described above for operator pattern costs

The modulus assignment operator, '%=', takes the value of the expression on the right, moduluses the value of the variable on the left by it, and sets the variable on the left to that value. It returns the new value of the variable. The left term must be a variable.

#### Assignment

Patterns Used: As described above for operator pattern costs, -4 if assigning a list element, +1 if it's not assigning a list element and it is assigning a local variable

The assignment operator, '=', sets the variable on the left to the value of the expression on the right. It returns the new value of the variable. The left term must be a variable.

#### OR

Patterns Used: 1

If given bools, returns true if either is true, and false otherwise. If given numbers, returns the bitwise OR of the two numbers. If given sets, returns the union of them.

#### AND

Patterns Used: 1

If given bools, returns true if both are true, and false otherwise. If given numbers, returns the bitwise AND of the two numbers. If given sets, returns the intersection of them.

#### Bitwise XOR

Patterns Used: 1

If given numbers, returns the bitwise AND of the two numbers. If given sets, returns the exclusive disjunction of them. If you want to take the XOR of two bools, use '!=' instead.

#### Equals

Patterns Used: 1

Returns true if the two values are the same (within a small tolerance).

#### Not Equals

Patterns Used: 1

Returns true if the two values are different (within a small tolerance).

#### Greater Than Or Equals

Patterns Used: 1

Returns true if the first number is greater than or equal to the second number.

#### Less Than Or Equals

Patterns Used: 1

Returns true if the first number is less than or equal to the second number.

#### Greater Than

Patterns Used: 1

Returns true if the first number is greater than the second number.

#### Less Than

Patterns Used: 1

Returns true if the first number is less than the second number.

#### Addition

Patterns Used: 1

If given numbers or vectors, returns the them added together. If given lists, returns the first list with the second list concatonated onto the end.

#### Subtraction

Patterns Used: 1

Returns the given number or vector on the left with the given number or vector on the right subtracted from it.

#### Multiplication

Patterns Used: 1

If given a number and a vector or number, returns them multiplied together. If given two vectors, returns their dot product.

#### Division

Patterns Used: 1

If given two numbers, returns the first number divided by the second. If given a number and a vector, returns the vector divided by the number. If given two vectors, returns their cross product.

#### Modulus

Patterns Used: 1

Returns the first number or vector modulus the second number or vector. In the case of two vectors, operation is done element-wise.
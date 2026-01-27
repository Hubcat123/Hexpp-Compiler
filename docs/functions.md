# Functions

There are two kinds of functions in Hex++. These are functions declared by you, and inbuilt functions. This document will go over what declaring a funciton is and how it works, and then go over all in-built functions and what they do. Calling functions works identically between declared functions and inbuilt functions, and is described in detail in statements.md.

## Declared Functions

Declared functions are what encapsulates code your program will be running. All Hex++ programs start with the main function, defined like so:
```
void main() {

}
```
Whatever code is written in the curly braces will be ran. Unlike all other functions, you cannot call main. However, other functions you define may be called in any other functions, including themselves and main. They cannot be called during the decleration of global variables, though.

There are two kinds of declared functions, void functions and non-void functions. Here's what the most basic declarations of these look like:
```
void foo() {

}

ret bar() {

}
```

### Void Functions

Functions prefixed with 'void' will be void functions. These do not return a value, meaning they cannot be used as part of expressions. These are what you should use whenever you want your program to do something. You can return from these functions, which will cause them to stop executing whatever code they were, and resume executing the code wherever they were called. Here's an example of what a void function may do, using a few inbuilt functions:
```
void conjure_block_below{
    // Get the postition of the block below the caster
    let block_below = self().pos() + vec(0, -1, 0);

    /* If a raycast from the block below us ends hitting the block below
    us, then there's no space to conjure a block so we return */
    if (block_raycast(block_below, vec(0, 1, 0)) == block_below) {
        return;
    }

    // Conjure the block
    create_block(block_below);
}
```
This function will check the space below the caster, and if there's an open block there, will conjure a block.

### Non-Void Functions

Functions prefixed with 'ret' will be non-void functions. These return a value, meaning that they can be used as parts of expressions. These functions can do things, like void funcitons, but they can also return information out of them. To return from them, you use the 'return' keyword, much like void functions. However now, you give them something to return. Here's an example, using a few inbuilt functions:
```
ret get_tallest_nearby_entity() {
    // Initialize variables for keeping track of tallest entity
    let tallest_entity = null;
    let tallest_entity_height = 0;

    // Get nearby entities
    let nearby_entities = get_entities(self().pos(), 6);

    // Loop over all entities
    let i = nearby_entities.size();
    while (i > 0)
    {
        --i;

        // Check if current entity is taller than prev tallest
        let curr_height = nearby_entities[i].height();
        if (curr_height > tallest_entity_height)
        {
            tallest_entity_height = curr_height;
            talles_entity = nearby_entities[i];
        }
    }

    return tallest_entity;
}
```
In this case, the function will look at all entities within 6 blocks of the caster, and return the entity that is tallest. An important note is that if a non-void function reaches the end without hitting a return statement, it will return null. In general, it is suggested you avoid allowing this fail-safe to activate, as it can make the code less clear.

### Parameters

Functions can also have information passed into them. This can be useful for flexibly using functions in different circumstances. Parameters are defined when the function is declared, and then are passed in when the function is called. They can exist for both void and non-void functions. Here's an example:
```
// Gets the distance between two points
void distance(pos1, pos2) {
    // Get the vector from pos 2 to pos 1, and square each coord
    let vec_between_sqr = pow(pos1 - pos2, 2);

    // Sum all the coords together
    let sum = vec_between_sqr.x + vec_between_sqr.y + vec_between_sqr.z;

    // Return sqrt of sum
    return pow(sum, 1 / 2);
}

void main() {
    // Get the location of the block the caster is looking at
    let block = block_raycast(self());

    // Null check
    if (block != null)
    {
        /* Pass in the caster's eye position and the block's position to
        print distance */
        print(distance(self().eye_pos(), block));
    }
}
```
This program will print the distance from the caster's eyes to the block they're looking at.

## Inbuilt Functions

There are a large number of inbuilt funcitons in Hex++ that are used for many things. Below is a comprehensive list, sorted into sections.
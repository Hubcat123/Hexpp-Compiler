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

There are a large number of inbuilt funcitons in Hex++ that are used for many things. Below is a comprehensive list, sorted into sections. All inbuilt functions list "Related Pattern(s)" which are base Hex Casting patterns related to the function's use. If you have a pattern who's use you want to emulate in Hex++, try searching for it's name here.

### Sections

This is a list of all the sections below. Some sections may be split further into smaller sections.

1. Lists
2. Vectors
3. World Info
4. Info Refining
5. Math
6. Reading & Writing
7. Meta Execution
8. Meta Info
9.   Spells

### 1. Lists

List functions are all member functions, meaning they are called by using the '.' operator on a list.

#### ret with(any)
Related Pattern(s): Integration Distillation

Patterns Used: 1

Aliases: with_back(any)

Returns a copy of the list with the given element appended to the end.

#### ret with_front(any)
Related Pattern(s): Speaker's Distillation

Patterns Used: 1

Returns a copy of the list with the given element inserted at the front.

#### ret sublist(num, num)
Related Pattern(s): Selection Exaltation

Patterns Used: 1

Returns a sublist of the list from the start index (inclusive) to end index (exclusive).

#### ret back()
Related Pattern(s): Derivation Decomposition

Patterns Used: 2

Returns the last element of the list.

#### ret reversed()
Related Pattern(s): Retrograde Purification

Patterns Used: 1

Returns a copy of the list with all elements in reverse order.

#### ret without_at(num)
Related Pattern(s): Excisor Distillation

Patterns Used: 1

Returns a copy of the list with the element at the given index removed.

#### ret without_duplicates()
Related Pattern(s): Uniqueness Purification

Patterns Used: 1

Returns a copy of the list with duplicate elements removed.

#### ret front()
Related Pattern(s): Speaker's Decomposition

Patterns Used: 2

Returns the first element of the list.

#### ret find(any)
Related Pattern(s): Locator's Distillation

Patterns Used: 1

Returns the index of the first occurrence of the given element in the list, or -1 if not found.

#### ret size()
Related Pattern(s): Length Purification

Patterns Used: 1

Returns the number of elements in the list.


### 2. Vectors

#### ret vec(num, num, num)
Related Pattern(s): Vector Exaltation

Patterns Used: 1

Creates a vector with the given x, y, z components.

If given all 0's, or all 0's except one 1 or -1, then the compiler will simplify it to 1 pattern instead of 4 (1 for vec, 3 for the numbers).

#### ret vec0()
Related Pattern(s): Vector Reflection Zero

Patterns Used: 1

Creates a zero vector (0, 0, 0).

#### ret vecXP()
Related Pattern(s): Vector Reflection

Patterns Used: 1

Creates a unit vector pointing in the positive X direction (1, 0, 0).

#### ret vecXN()
Related Pattern(s): Vector Reflection

Patterns Used: 1

Creates a unit vector pointing in the negative X direction (-1, 0, 0).

#### ret vecYP(); ret vec_up()
Related Pattern(s): Vector Reflection

Patterns Used: 1

Creates a unit vector pointing in the positive Y direction (0, 1, 0).

#### ret vecYN(); ret vec_down()
Related Pattern(s): Vector Reflection

Patterns Used: 1

Creates a unit vector pointing in the negative Y direction (0, -1, 0).

#### ret vecZP()
Related Pattern(s): Vector Reflection

Patterns Used: 1

Creates a unit vector pointing in the positive Z direction (0, 0, 1).

#### ret vecZN()
Related Pattern(s): Vector Reflection

Patterns Used: 1

Creates a unit vector pointing in the negative Z direction (0, 0, -1).

#### ret x()
Related Pattern(s): Vector Disintegration

Patterns Used: 2

Returns the x component of the vector.

#### ret y()
Related Pattern(s): Vector Disintegration

Patterns Used: 2

Returns the y component of the vector.

#### ret z()
Related Pattern(s): Vector Disintegration

Patterns Used: 2

Returns the z component of the vector.

#### ret length()
Related Pattern(s): Length Purification

Patterns Used: 1

Returns the magnitude (length) of the vector.

### 3. World Info

#### ret self()
Related Pattern(s): Mind's Reflection

Patterns Used: 1

Returns the entity object representing the caster.

#### ret block_raycast(pos, dir)
Related Pattern(s): Archer's Distillation

Patterns Used: 1

Performs a raycast from a position in a direction and returns the position of the block hit, or null if no block is hit.

#### ret block_raycast(entity)
Related Pattern(s): Archer's Distillation

Patterns Used: 5

Performs a raycast from an entity's eye position in the direction they are looking and returns the position of the block hit.

#### ret block_normal_raycast(pos, dir)
Related Pattern(s): Architect's Distillation

Patterns Used: 1

Performs a raycast from a position in a direction and returns the normal vector of the block face hit, or null if no block is hit.

#### ret block_normal_raycast(entity)
Related Pattern(s): Architect's Distillation

Patterns Used: 5

Performs a raycast from an entity's eye position and returns the normal vector of the block face hit.

#### ret entity_raycast(pos, dir)
Related Pattern(s): Scout's Distillation

Patterns Used: 1

Performs a raycast from a position in a direction and returns the entity hit, or null if no entity is hit.

#### ret entity_raycast(entity)
Related Pattern(s): Scout's Distillation

Patterns Used: 5

Performs a raycast from an entity's eye position in the direction they are looking and returns the entity hit.

#### ret get_entity(pos)
Related Pattern(s): Entity Purification

Patterns Used: 1

Returns the entity at the specified position, or null if no entity exists there.

#### ret get_entities(pos, radius)
Related Pattern(s): Zone Distillation (Any)

Patterns Used: 1

Returns a list of all entities within the specified radius of the given position.

#### ret get_animal(pos)
Related Pattern(s): Entity Purification (Animal)

Patterns Used: 1

Returns the animal at the specified position, or null if no animal exists there.

#### ret get_animals(pos, radius)
Related Pattern(s): Zone Distillation (Animal)

Patterns Used: 1

Returns a list of all animals within the specified radius of the given position.

#### ret get_monster(pos)
Related Pattern(s): Entity Purification (Monster)

Patterns Used: 1

Returns the monster at the specified position, or null if no monster exists there.

#### ret get_monsters(pos, radius)
Related Pattern(s): Zone Distillation (Monster)

Patterns Used: 1

Returns a list of all monsters within the specified radius of the given position.

#### ret get_item(pos)
Related Pattern(s): Entity Purification (Item)

Patterns Used: 1

Returns the item entity at the specified position, or null if no item entity exists there.

#### ret get_items(pos, radius)
Related Pattern(s): Zone Distillation (Item)

Patterns Used: 1

Returns a list of all item entities within the specified radius of the given position.

#### ret get_player(pos)
Related Pattern(s): Entity Purification (Player)

Patterns Used: 1

Returns the player at the specified position, or null if no player exists there.

#### ret get_players(pos, radius)
Related Pattern(s): Zone Distillation (Player)

Patterns Used: 1

Returns a list of all players within the specified radius of the given position.

#### ret get_living(pos); ret get_living(pos, radius)
Related Pattern(s): Entity Purification (Living), Zone Distillation (Living)

Patterns Used: 1

Returns a living entity at the position, or a list of all living entities within the radius.

#### ret get_non_animals(pos, radius)
Related Pattern(s): Zone Distillation (Non-Animal)

Patterns Used: 1

Returns a list of all non-animal entities within the specified radius of the given position.

#### ret get_non_monsters(pos, radius)
Related Pattern(s): Zone Distillation (Non-Monster)

Patterns Used: 1

Returns a list of all non-monster entities within the specified radius of the given position.

#### ret get_non_items(pos, radius)
Related Pattern(s): Zone Distillation (Non-Item)

Patterns Used: 1

Returns a list of all non-item entities within the specified radius of the given position.

#### ret get_non_players(pos, radius)
Related Pattern(s): Zone Distillation (Non-Player)

Patterns Used: 1

Returns a list of all non-player entities within the specified radius of the given position.

#### ret get_non_living(pos, radius)
Related Pattern(s): Zone Distillation (Non-Living)

Patterns Used: 1

Returns a list of all non-living entities within the specified radius of the given position.

#### ret circle_impetus_pos()
Related Pattern(s): Waystone Reflection

Patterns Used: 1

Returns the position of the spell circle's impetus.

#### ret circle_impetus_forward()
Related Pattern(s): Lodestone Reflection

Patterns Used: 1

Returns the forward direction of the spell circle's impetus.

#### ret circle_LNW()
Related Pattern(s): Lesser Fold Reflection

Patterns Used: 1

Returns the lower north-west corner of the spell circle's bounds.

#### ret circle_USE()
Related Pattern(s): Greater Fold Reflection

Patterns Used: 1

Returns the upper south-east corner of the spell circle's bounds.

### 4. Info Refining

Info refining functions are all member functions, meaning they are called by using the '.' operator on a list.

#### ret pos()
Related Pattern(s): Compass Purification II

Patterns Used: 1

Returns the position (x, y, z vector) of the entity.

#### ret eye_pos()
Related Pattern(s): Compass Purification

Patterns Used: 1

Returns the eye position (x, y, z vector) of the entity.

#### ret height()
Related Pattern(s): Stadiometer's Purification

Patterns Used: 1

Returns the height of the entity.

#### ret velocity()
Related Pattern(s): Pace Purification

Patterns Used: 1

Returns the velocity vector of the entity.

#### ret forward()
Related Pattern(s): Alidade's Purification

Patterns Used: 1

Returns the forward direction vector of the entity.

#### ret is_flying(entity)
Related Pattern(s): Aviator's Purification

Patterns Used: 1

Returns true if the entity is currently flying, false otherwise.

#### ret sign()
Related Pattern(s): Axial Purification

Patterns Used: 1

If called on a number, returns the sign of the number (-1, 0, or 1). If called on a vector, returns the nearest axis vector to that vector as a unit vector.

### 5. Math

#### Basic Math

##### ret pow(num, num)
Related Pattern(s): Power Distillation

Patterns Used: 1

Returns the result of raising the first number to the power of the second number.

##### ret floor(num)
Related Pattern(s): Floor Purification

Patterns Used: 1

Returns the number rounded down.

##### ret ceil(num)
Related Pattern(s): Ceiling Purification

Patterns Used: 1

Returns the number rounded up.

##### ret min(num, num)
Related Pattern(s): Minimus Distillation

Patterns Used: 4

Returns the smaller of two numbers.

##### ret max(num, num)
Related Pattern(s): Maximus Distillation

Patterns Used: 4

Returns the larger of two numbers.

##### ret as_bool(any)
Related Pattern(s): Augur's Purification

Patterns Used: 1

Converts a value to a boolean. Null, False, 0, and empty lists are false; everything else is true.

##### ret abs(num)
Related Pattern(s): Length Purification

Patterns Used: 1

Returns the absolute value of the number.

##### ret random()
Related Pattern(s): Entropy Reflection

Patterns Used: 1

Returns a random number between 0 and 1.

##### ret random(num, num)
Related Pattern(s): Entropy Reflection

Patterns Used: 5

Returns a random number between the first number and second number.

##### ret tau()
Related Pattern(s): Circle Reflection

Patterns Used: 1

Returns tau (2π ≈ 6.283185307179586).

##### ret pi()
Related Pattern(s): Arc's Reflection

Patterns Used: 1

Returns pi (π ≈ 3.141592653589793).

##### ret e()
Related Pattern(s): Euler's Reflection

Patterns Used: 1

Returns Euler's number (e ≈ 2.718281828459045).

#### Trigonometry

##### ret sin(num)
Related Pattern(s): Sine Purification

Patterns Used: 1

Returns the sine of the angle in radians.

##### ret cos(num)
Related Pattern(s): Cosine Purification

Patterns Used: 1

Returns the cosine of the angle in radians.

##### ret tan(num)
Related Pattern(s): Tangent Purification

Patterns Used: 1

Returns the tangent of the angle in radians.

##### ret arc_sin(num)
Related Pattern(s): Inverse Sine Purification

Patterns Used: 1

Returns the arcsine of the value in radians.

##### ret arc_cos(num)
Related Pattern(s): Inverse Cosine Purification

Patterns Used: 1

Returns the arccosine of the value in radians.

##### ret arc_tan(num)
Related Pattern(s): Inverse Tangent Purification

Patterns Used: 1

Returns the arctangent of the value in radians.

##### ret angle(num, num)
Related Pattern(s): Inverse Tangent Distillation

Patterns Used: 1

Returns the angle in radians of the vector (x, y) from the x-axis.

##### ret log(num, num)
Related Pattern(s): Logarithmic Distillation

Patterns Used: 1

Returns the logarithm of the first value with the second value as the base.

##### ret ln(num)
Related Pattern(s): Euler's Reflection, Logarithmic Distillation

Patterns Used: 2

Returns the natural logarithm (base e) of the value.



### 6. Reading & Writing

#### void print(any)
Related Pattern(s): Reveal

Patterns Used: 2

Prints the data to the caster's chat.

#### void write(any); void write(item entity, any)
Related Pattern(s): Scribe's Gambit, Chronicler's Gambit

Patterns Used: 1

The write function takes in a piece data passed into the last parameter, and either writes it into the item in your off-hand, or if passed an item entity as well, writes it into the item entity.

#### void write_akashic(pos, pattern, any)
Related Pattern(s): Akasha's Gambit

Patterns Used: 1

Writes the given data to the key of the given pattern in the akashic record at the given position.

#### ret read(); ret read(item entity)
Related Pattern(s): Scribe's Reflection, Chronicler's Purification

Patterns Used: 1

Reads data from the item in your off-hand, or from the specified item entity.

#### ret can_read(); ret can_read(item entity)
Related Pattern(s): Auditor's Reflection, Auditor's Purification

Patterns Used: 1

Returns true if the item in your off-hand or the specified item entity can be read from.

#### ret can_write(); ret can_write(item entity)
Related Pattern(s): Assessor's Reflection, Assessor's Purification

Patterns Used: 1

Returns true if the item in your off-hand or the specified item entity can be written to.

#### ret read_akashic(pos, pattern)
Related Pattern(s): Akasha's Distillation

Patterns Used: 1

Returns data from the key of the given pattern in the akashic record at the given position.

##### void erase_item()
Related Pattern(s): Erase Item

Patterns Used: 1

Erases the data of the item in your off-hand. Works on casting items.

### 7. Meta Execution

#### ret execute(patterns); ret execute(patterns, any)
Related Pattern(s): Hermes' Gambit

Patterns Used: (13; 17), plus however many patterns it executes

Executes the given patterns in a simulated spell environment. The raven's mind will be null, and the stack will be empty, unless some additional data is passed in, in which case it will be the only thing on the stack. A list containing all iotas on the stack at the end of execution will be returned.

#### ret execute_no_ravens_mind(patterns); ret execute_no_ravens_mind(patterns, any)
Related Pattern(s): Hermes' Gambit

Patterns Used: (7; 11), plus however many patterns it executes

Executes the given patterns in a simulated spell environment. The raven's mind will not be null, and must be the same at the end of execution as it was at the beginning. The stack will be empty, unless some additional data is passed in, in which case it will be the only thing on the stack. A list containing all iotas on the stack at the end of execution will be returned.

#### void execute_unsafe(patterns); void execute_unsafe(patterns, any)
Related Pattern(s): Hermes' Gambit

Patterns Used: (1; 2), plus however many patterns it executes

Executes the given patterns directly on the spell's stack. Using this function requires caution with the pattern's usage of the stack, but the advantages are that the pattern cost is much lower, and that the spell's stack can be accessed and manipulated if needed. If data is passed in, that data is placed on top of the stack.

The patterns passed into this function must follow these rules to prevent errors in the spell:

1. The raven's mind should be the same by the end of the patterns as it was at the start.
2. The patterns should only mess with iotas in it's "stack space" (This being the (potential) data passed into the function, as well as any iotas the patterns themselves placed there).
3. By the time the patterns finish executing, there should be nothing left in it's stack space, including any potenially passed-in data, except one iota left on top. This iota is what will be returned from the function.

You should only break these rules if you really know what you're doing, which requires knowledge of how the compiler does things under the hood.

#### void execute_unsafe_no_ret(patterns); void execute_unsafe_no_ret(patterns, any)
Related Pattern(s): Hermes' Gambit

Patterns Used: (1; 2), plus however many patterns it executes

Works the same as execute_unsafe, but the pattern's "stack space" should be empty, and nothing is returned from the function. Please read execute_unsafe's description before attempting use.

### 8. Meta Info

#### ret patterns_remaining()
Related Pattern(s): Thanatos' Reflection

Patterns Used: 1

Returns the number of patterns remaining that can be executed before hitting the pattern limit (100,000).

#### ret stack_size()
Related Pattern(s): Flock's Reflection

Patterns Used: 1

Returns the current size of the stack. Mostly useful for debugging or avoiding stack filling mishaps.

#### void dump_stack()
Related Pattern(s): None

Patterns Used: 7

Outputs a list of the current stack for debugging purposes.

#### void dump_ravens_mind()
Related Pattern(s): Muninn's Reflection

Patterns Used: 1

Outputs the current Raven's Mind for debugging purposes.

### 9. Spells

#### Regular Spells

##### void mine(pos)
Related Pattern(s): Break Block

Patterns Used: 1

Destroys the block at the given position.

##### void place_block(pos)
Related Pattern(s): Place Block

Patterns Used: 1

Places a block at the given position using materials from the caster's inventory. The block placed is the same as the "Place Block" pattern.

##### void destroy_liquid(pos)
Related Pattern(s): Destroy Liquid

Patterns Used: 1

Destroys liquid near the given position.

##### void destroy_fire(pos)
Related Pattern(s): Extinguish Area

Patterns Used: 1

Extinguishes fire near the given position.

##### void create_block(pos)
Related Pattern(s): Conjure Block

Patterns Used: 1

Conjures a block at the given position.

##### void create_fire(pos)
Related Pattern(s): Ignite

Patterns Used: 1

Ignites fire on top of the block at the given position.

##### void create_explosion(pos, num)
Related Pattern(s): Explosion

Patterns Used: 1

Creates an explosion at the given position with the specified power.

##### void create_explosion_fire(pos, num)
Related Pattern(s): Fireball

Patterns Used: 1

Creates a firey explosion at the given position with the specified power.

##### void create_light(pos)
Related Pattern(s): Conjure Light

Patterns Used: 1

Creates a light source at the given position.

##### void create_water(pos)
Related Pattern(s): Create Water

Patterns Used: 1

Creates water at the given position.

##### void add_vel(entity, vec)
Related Pattern(s): Impulse

Patterns Used: 1

Adds velocity to an entity in the given direction with the magnitude of the given direction.

##### void teleport_forward(entity, num)
Related Pattern(s): Blink

Patterns Used: 1

Teleports an entity forward by the specified distance in the direction they're facing. Also cancels momentum of the entity.

##### void play_note(pos, num, num)
Related Pattern(s): Make Note

Patterns Used: 1

Plays a note with the specified instrument and note value at the given position. Instrument and note values are found the same way as the "Make Note" pattern.

##### void fly_range(entity, num)
Related Pattern(s): Anchorite's Flight

Patterns Used: 1

Enables flight for an entity with a range limit in blocks.

##### void fly_duration(entity, num)
Related Pattern(s): Wayfarer's Flight

Patterns Used: 1

Enables flight for an entity with a duration limit in seconds.

##### void change_color()
Related Pattern(s): Internalize Pigment

Patterns Used: 1

Changes the caster's spell color using the pigment in their offhand.

##### void change_shape()
Related Pattern(s): Caster's Glamour

Patterns Used: 1

Changes the shape of the item in the caster's offhand. Works the same as the pattern "Caster's Glamour."

##### void grow(pos)
Related Pattern(s): Overgrow

Patterns Used: 1

Applies bonemeal to a plant at the given position.

##### void edify(pos)
Related Pattern(s): Edify Sapling

Patterns Used: 1

Edifies a sapling at the given position.

##### void create_sentinel(pos)
Related Pattern(s): Summon Sentinel

Patterns Used: 1

Summons a sentinel at the given position, marking that spot for spells to access.

##### void destroy_sentinel()
Related Pattern(s): Banish Sentinel

Patterns Used: 1

Destroys the active sentinel.

##### ret sentinel_pos()
Related Pattern(s): Locate Sentinel

Patterns Used: 1

Returns the position of the active sentinel.

##### ret sentinel_dir_from(pos)
Related Pattern(s): Wayfind Sentinel

Patterns Used: 1

Returns the direction from the given position to the active sentinel.

##### void effect_weakness(entity, num, num)
Related Pattern(s): White Sun's Nadir

Patterns Used: 1

Applies the Weakness effect to an entity with the given duration and amplifier.

##### void effect_levitation(entity, num)
Related Pattern(s): Blue Sun's Nadir

Patterns Used: 1

Applies the Levitation effect to an entity with the given duration.

##### void effect_withering(entity, num, num)
Related Pattern(s): Black Sun's Nadir

Patterns Used: 1

Applies the Wither effect to an entity with the given duration and amplifier.

##### void effect_poison(entity, num, num)
Related Pattern(s): Red Sun's Nadir

Patterns Used: 1

Applies the Poison effect to an entity with the given duration and amplifier.

##### void effect_slowness(entity, num, num)
Related Pattern(s): Green Sun's Nadir

Patterns Used: 1

Applies the Slowness effect to an entity with the given duration and amplifier.

##### void craft_cypher(item entity, patterns)
Related Pattern(s): Craft Cypher

Patterns Used: 1

Turns an empty cypher in the caster's offhand into a cypher with a media capacity equal to the item entity (which is consumed), and with a spell which is the patterns passed in.

##### void craft_trinket(any, num)
Related Pattern(s): Craft Trinket

Patterns Used: 1

Turns an empty trinket in the caster's offhand into a trinket with a media capacity equal to the item entity (which is consumed), and with a spell which is the patterns passed in.

##### void craft_artifact(any, num)
Related Pattern(s): Craft Artifact

Patterns Used: 1

Turns an empty artifact in the caster's offhand into a artifact with a media capacity equal to the item entity (which is consumed), and with a spell which is the patterns passed in.

##### void recharge_item(item entity)
Related Pattern(s): Recharge Item

Patterns Used: 1

Consumes the passed item entity and recharges an item in the caster's offhand with the media value of the passed entity.

#### Great Spells

Since Great Spells have unique patterns in every world, their use requires changing some config options for Hexagon, if it's being used to generate a /give command. For detailed instructions, check out the README's section on Hexagon.

##### void craft_phial(item entity)
Related Pattern(s): Craft Phial

Patterns Used: 1

Crafts a media phial with capacity equal to the media in the passed item entity (which is consumed).

##### void flay_mind(entity, pos)
Related Pattern(s): Flay Mind

Patterns Used: 1

I cannot make heads or tails of this function... To be honest, I'm not sure I want to know what it does.

##### void weather_rain()
Related Pattern(s): Summon Rain

Patterns Used: 1

Sets the weather to be raining or snowing.

##### void weather_clear()
Related Pattern(s): Dispel Rain

Patterns Used: 1

Clears the weather, stopping rain or snow.

##### void fly_wings(player)
Related Pattern(s): Altiora

Patterns Used: 1

Gives the player elytra until the land, and launches them into the air.

##### void teleport_relative(entity, vec)
Related Pattern(s): Greater Teleport

Patterns Used: 1

Teleports an entity by the offset vector relative to their current position.

##### void teleport_to(entity, pos)
Related Pattern(s): Greater Teleport

Patterns Used: 4

Teleports an entity to the given destination position.

##### void create_greater_sentinel(pos)
Related Pattern(s): Summon Greater Sentinel

Patterns Used: 1

Summons a greater sentinel at the given position. Spells may affect things within 16 blocks of your greater sentinel.

##### void create_lightning(pos)
Related Pattern(s): Summon Lightning

Patterns Used: 1

Summons lightning at the given position.

##### void create_lava(pos)
Related Pattern(s): Create Lava

Patterns Used: 1

Creates lava at the given position.

##### void effect_regeneration(entity, num, num)
Related Pattern(s): White Sun's Zenith

Patterns Used: 1

Applies the Regeneration effect to an entity with the given duration and amplifier.

##### void effect_night_vision(entity, num)
Related Pattern(s): Blue Sun's Zenith

Patterns Used: 1

Applies the Night Vision effect to an entity with the given duration.

##### void effect_absorption(entity, num, num)
Related Pattern(s): Black Sun's Zenith

Patterns Used: 1

Applies the Absorption effect to an entity with the given duration and amplifier.

##### void effect_haste(entity, num, num)
Related Pattern(s): Red Sun's Zenith

Patterns Used: 1

Applies the Haste effect to an entity with the given duration and amplifier.

##### void effect_strength(entity, num, num)
Related Pattern(s): Green Sun's Zenith

Patterns Used: 1

Applies the Strength effect to an entity with the given duration and amplifier.
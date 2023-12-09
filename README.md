# mystack
Educational project at MIPT. My implementation of stack, featuring canary and hash protection, as well as ability to use any data types.

## Brief description
The whole library consists of one file `stack.h` for convenience, so it is very easy to include it in your projects. You can change some behaviour by including corresponding defines 
before `#include "stack.h"` (see below).

## Usage
If you want to try it out, just copy `stack.h` in your headers' folder and include it where you want. Please note that `main.cpp` is not a part of the library, but it has an example of usage. 
You also need to specify the data type you are going to store in the stack. It is done as follows:

- Write `typedef *your type* Elem_t;` _**before**_ the line `#include "stack.h"`. For example: `typedef double Elem_t;`.
- Specify the function which prints a value of your type in stream; it is used for dumps. For example: `void inline print_elem_t(FILE *stream, Elem_t val) { fprintf(stream, "%lf", val); }`.
- Note that this printing function must have the signature just like in the example above, including the name.

You can define the following macros to change the behaviour of your stack:

- `STACK_DO_DUMP` Turns on dumps and verifications.
- `STACK_USE_POISON` Turns on using poison values for filling empty parts of stack; can be useful for debugging and can be seen in dumps.
- `STACK_ABORT_ON_DUMP` Turns on calling abort() in the end of the dump.
- `STACK_DUMP_ON_INVALID_POP` Turns on calling dump when invalid pop is detected (pop() is called, but stack is empty).
- `STACK_USE_PROTECTION_CANARY` Turns on using canary protection.
- `STACK_USE_PROTECTION_HASH` Turns on using hash protection.
- `STACK_FULL_DEBUG_INFO` Turns on printing the most of debug info, not only in dumps.

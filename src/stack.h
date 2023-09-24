#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include <time.h>

//ВРЕМЕННО, по хорошему это должно быть перед include stack.h
//typedef int Elem_t;
//#define ELEM_T_SPECF "%d"

/*
    USED DEFINES:
    1) STACK_DO_DUMP
    2) STACK_USE_POISON
    3) EXIT_ON_DUMP
*/

//--------------------------------------------------------------------------------------------

typedef unsigned char poison_t;
const poison_t POISON_VALUE = (poison_t) 0xBE;

typedef long int stacksize_t;
#define STACKSIZE_T_SPECF "%ld"

/*
    ------------------------------------TODO--------------------------------------
    3) всё под условную компиляцию!! и под разную!! и не использовать NDEBUG!!

    4) TODO разбросанные по коду
*/

//! @brief Holds values returned by funcs like stack_pop(), stack_push(), stack_ctor(), etc.
//! @note If STACK_ERROR_VERIFY is returned, you can call stack_verify() on your own
//! to see details. Also STACK_DUMP() is called automtaically if NDEBUG is not defined.
enum StackErrorCode
{
    STACK_ERROR_NO_ERROR            = 0, //< No errors occurred.
    STACK_ERROR_NULL_STK_PNT_PASSED = 1, //< NULL pointer to stack was passed.
    STACK_ERROR_VERIFY              = 2, //< stack_verify() returned non-zero value.
    STACK_ERROR_NULL_RET_VALUE_PNT  = 3, //< NULL passed as a pointer to the return value.
    STACK_ERROR_MEM_BAD_REALLOC     = 4, //< Stack reallocation failed.
    STACK_ERROR_NOTHING_TO_POP      = 5, //< Stack is empty, but pop() was called.
};

//! @brief Mask consisting of values of this enum is returned by stack_verify().
//! @note size may equal capacity, but next push() will call realloc().
//! @note Data pointer equalling NULL is considered fine only if
//! size == 0 and capacity == 0!
enum StackVerifyResFlag
{
    STACK_VERIFY_NULL_PNT           = 1, //< Passed pointer to the stack is NULL.
    STACK_VERIFY_DATA_PNT_WRONG     = 2, //< Pointer to data is NULL and either size != 0 or capacity != 0).
    STACK_VERIFY_SIZE_INVALID       = 4, //< Size < 0 or size > capacity.
    STACK_VERIFY_CAPACITY_INVALID   = 8, //< capacity < 0.
};

struct Stack
{
    Elem_t *data = NULL;
    stacksize_t size = -1;
    stacksize_t capacity = -1;

    const char *stack_name = NULL;
    const char *orig_file_name = NULL;
    int orig_line = -1;
    const char *orig_func_name = NULL;
};

//! @brief Checks stack's condition.
//! @param [in] stk Stack to check.
//! @return Mask composed from StackVerifyResFlag enum values, equaling 0 if the stack is fine.
static int stack_verify(const Stack *stk);

//! @brief Stack constructor. ONLY FOR INTERNAL USE! USE MACRO stack_ctor()!
//! @details It doesn't allocate memory, setting size and capacity equalling 0. But
//! first push() will lead to realloc_up().
//! @param [in] stk Pointer to stack to construct.
//! @return StackErrorCode enum value.
static StackErrorCode stack_ctor_(  Stack *stk,
                                    const char *stack_name,
                                    const char *orig_file_name,
                                    const int orig_line,
                                    const char *orig_func_name);

//! @brief Stack deconstructor.
//! @param [in] stk Pointer to stack to deconstruct.
//! @return StackErrorCode enum value.
static StackErrorCode stack_dtor(Stack *stk);

//! @brief Pushes element to stack.
//! @param [in] stk Pointer to the stack.
//! @param [in] value Value to push to the stack.
//! @return StackErrorCode enum value.
static StackErrorCode stack_push(Stack *stk, Elem_t value);

//! @brief Pops element from stack.
//! @param [in] stk Pointer to the stack.
//! @param [in] ret_value Pointer to put popped value to.
//! @return StackErrorCode enum value.
static StackErrorCode stack_pop(Stack *stk, Elem_t *ret_value);

//! @brief Checks stack's state and, if needed, reallocs memory for the stack and changes stk->data.
//! @param [in] stk Pointer to the stack.
//! @return StackErrorCode enum value.
static StackErrorCode stack_realloc(Stack *stk);

#ifndef STACK_DO_DUMP

#define STACK_DUMP(stk, verify_res) (void(0))

#else  //STACK_DO_DUMP is turned on

#define STACK_DUMP(stk, verify_res) stack_dump_( (stk), verify_res, __FILE__, __LINE__)

static void stack_dump_(Stack *stk, int verify_res, const char *file, int line);

#endif //STACK_DO_DUMP

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------STACK.CPP---------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

int stack_verify(const Stack *stk)
{
    int error = 0;

    if ( !stk )                                                         error |= STACK_VERIFY_NULL_PNT;

    if ( stk && !(stk->data) && (stk->size != 0 || stk->capacity != 0)) error |= STACK_VERIFY_DATA_PNT_WRONG;

    if ( stk && (stk->size < 0 || stk->size > (stk->capacity) ) )       error |= STACK_VERIFY_SIZE_INVALID;

    if ( stk && stk->capacity < 0)                                      error |= STACK_VERIFY_CAPACITY_INVALID;

    return error;
}

#define stack_ctor(stk) stack_ctor_(stk, #stk, __FILE__, __LINE__, __func__)

StackErrorCode stack_ctor_( Stack *stk,
                            const char *stack_name,
                            const char *orig_file_name,
                            const int orig_line,
                            const char *orig_func_name)
{
    if (!stk) return STACK_ERROR_NULL_STK_PNT_PASSED;

    stk->data = NULL;
    stk->capacity = 0;
    stk->size = 0;

    stk->stack_name = stack_name;
    stk->orig_file_name = orig_file_name;
    stk->orig_line = orig_line;
    stk->orig_func_name = orig_func_name;

    return STACK_ERROR_NO_ERROR;
}

StackErrorCode stack_dtor(Stack *stk)
{
    if (!stk) return STACK_ERROR_NULL_STK_PNT_PASSED;

    stk->capacity = -1;
    stk->size = -1;
    if (stk->data) free(stk->data);
    stk->data = NULL;

    stk->stack_name = NULL;
    stk->orig_file_name = NULL;
    stk->orig_line = -1;
    stk->orig_func_name = NULL;

    return STACK_ERROR_NO_ERROR;
}

#define STACK_CHECK(stk)    int verify_res = stack_verify(stk);     \
                            if ( verify_res != 0 ) {                \
                                STACK_DUMP(stk, verify_res);        \
                                return STACK_ERROR_VERIFY;          \
                            }

StackErrorCode stack_push(Stack *stk, Elem_t value)
{
    STACK_CHECK(stk)

    StackErrorCode mem_realloc_res = stack_realloc(stk); // сам stack_realloc определяет, нужно ли делать realloc
    if ( mem_realloc_res )
    {
        return mem_realloc_res;
    }

    (stk->data)[(stk->size)++] = value;

    return STACK_ERROR_NO_ERROR;
}

#ifdef STACK_USE_POISON
inline void fill_with_poison(Stack *stk, stacksize_t ind)
{
    assert(stk);
    assert(0 <= ind && ind < stk->capacity);

    poison_t *ptr = (poison_t *) (stk->data + ind);
    for (size_t i = 0; i < sizeof(Elem_t)/sizeof(poison_t); i++)
    {
        *ptr = POISON_VALUE;
        ptr += 1;
    }
}
#endif

StackErrorCode stack_pop(Stack *stk, Elem_t *ret_value)
{
    STACK_CHECK(stk)
    if ( !ret_value ) return STACK_ERROR_NULL_RET_VALUE_PNT;

    if (stk->size == 0) return STACK_ERROR_NOTHING_TO_POP;

    *ret_value = stk->data[--(stk->size)];

    #ifdef STACK_USE_POISON
    fill_with_poison(stk, stk->size);
    #endif

    return STACK_ERROR_NO_ERROR;
}

//-------------------------------------------------------------------------------------------------------

inline void fill_up_with_poison(Stack *stk, stacksize_t start_with_index)
{
    for (stacksize_t ind = start_with_index; ind < stk->capacity; ind++)
    {
        fill_with_poison(stk, ind);
    }
}

//! @brief Doubles (if MEM_MULTIPLIER == 2) the capacity of the stack, allocates new memory,
//! moves data to the new place, frees old memory. Supports case when data pointer
//! equals NULL and capacity == 0.
inline StackErrorCode stack_realloc_up__( Stack *stk, const int MEM_MULTIPLIER )
{
    if (stk->capacity == 0) stk->capacity = 1;

    Elem_t *new_data = (Elem_t *) calloc( (size_t) MEM_MULTIPLIER*((size_t) stk->capacity), sizeof(Elem_t) );
    if (!new_data) return STACK_ERROR_MEM_BAD_REALLOC;

    if (stk->data)
    {
        memcpy(new_data, stk->data, (size_t) stk->size);
    }

    if (stk->data)
    {
        free(stk->data);
    }

    stk->data = new_data;
    stk->capacity = MEM_MULTIPLIER * stk->capacity;

    #ifdef STACK_USE_POISON
    fill_up_with_poison(stk, stk->size);
    #endif

    return STACK_ERROR_NO_ERROR;
}

//! @brief Divides by two (if MEM_MULTIPLIER == 2) the capacity of the stack,
//! allocates new memory, moves data to the new place, frees old memory.
inline StackErrorCode stack_realloc_down__(Stack *stk, const int MEM_MULTIPLIER)
{
    Elem_t *new_data = (Elem_t *) calloc( (size_t) (stk->capacity) / MEM_MULTIPLIER, sizeof(Elem_t) );
    if (!new_data) return STACK_ERROR_MEM_BAD_REALLOC;

    if (stk->size > 0) memcpy(new_data, stk->data, (size_t) stk->size);

    free(stk->data);

    stk->data = new_data;
    stk->capacity = (stk->capacity) / MEM_MULTIPLIER;

    return STACK_ERROR_NO_ERROR;
}

StackErrorCode stack_realloc(Stack *stk)
{
    STACK_CHECK(stk)

    const int MEM_MULTIPLIER = 2;

    if ( stk->size >= stk->capacity )
    {
        StackErrorCode realloc_up_res = stack_realloc_up__(stk, MEM_MULTIPLIER);
        if (!realloc_up_res) return realloc_up_res;
    }
    else if ( stk->size <= stk->capacity / ( MEM_MULTIPLIER * MEM_MULTIPLIER ) )
    {
        StackErrorCode realloc_down_res = stack_realloc_down__(stk, MEM_MULTIPLIER);
        if (!realloc_down_res) return realloc_down_res;
    }

    return STACK_ERROR_NO_ERROR;
}

//-------------------------------------------------------------------------------------------------------

//#define STACK_DO_DUMP //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1111

#ifdef STACK_DO_DUMP

inline void stack_dump_verify_res_( int verify_res )
{
    //-------------------------------------------TODO-----------------------------------------
    fprintf(stderr, "Stack verification result: TEMPORARY LIKE THIS <%d>\n", verify_res);
}

inline void stack_dump_data_( Stack *stk )
{
    //const size_t N_FREE_SLOTS_TO_PRINT = 5; //TODO

    fprintf(stderr, "\t{\n");

    for (stacksize_t ind = 0; ind < stk->capacity; ind++)
    {
        fprintf(stderr, "\t\t[" STACKSIZE_T_SPECF "]\t = <" ELEM_T_SPECF ">", ind, stk->data[ind]);

        #ifdef STACK_USE_POISON
        if (ind >= stk->size)
        {
            fprintf(stderr, " (MAYBE POISON: %x)", stk->data[ind]);
        }
        #endif

        if (stk->size == ind) fprintf(stderr, " <--");

        fprintf(stderr, "\n");
    }

    fprintf(stderr, "\t}\n");
}

inline void print_curr_local_time(FILE *stream)
{
    time_t curr_time = time(NULL);
    tm curr_local_time = *localtime(&curr_time);


    fprintf(stream, "%d-%02d-%02d %02d:%02d:%02d",    curr_local_time.tm_year + 1900,
                                                        curr_local_time.tm_mon + 1,
                                                        curr_local_time.tm_mday,
                                                        curr_local_time.tm_hour,
                                                        curr_local_time.tm_min,
                                                        curr_local_time.tm_sec);
}

void stack_dump_(Stack *stk, int verify_res, const char *file, const int line)
{
    //TODO печать в log файл, а не в stderr
    fprintf(stderr, "STACK DUMP at ");
    print_curr_local_time(stderr);
    fprintf(stderr, "\n");

    fprintf(stderr, "Stack[%p] \"%s\" declared in %s(%d), in function %s. "
                    "STACK_DUMP() called from %s, on line %d.\n",    stk,
                                                        stk->stack_name,
                                                        stk->orig_file_name,
                                                        stk->orig_line,
                                                        stk->orig_func_name,
                                                        file, line);

    stack_dump_verify_res_(verify_res);

    if (!stk)
    {
        fprintf(stderr, "Stack pointer is NULL, no further information is accessible.\n");
        return;
    }

    fprintf(stderr,  "{\n"
                        "\tsize = <" STACKSIZE_T_SPECF ">\n"
                        "\tcapacity = <" STACKSIZE_T_SPECF ">\n"
                        "\tdata[%p]\n", stk->size, stk->capacity, stk->data );

    if ( !(stk->data) )
    {
        fprintf(stderr, "Data pointer is NULL. Data cannot be accessed.\n");
        return;
    }

    stack_dump_data_(stk);

    fprintf(stderr, "}\n");

    #ifdef EXIT_ON_DUMP
    exit(verify_res);
    #endif
}

#endif // STACK_DO_DUMP

#endif // STACK_H

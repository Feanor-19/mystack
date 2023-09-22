#include "stack.h"

#include <assert.h>
#include <memory.h>
#include <stdio.h>

int stack_verify(const Stack *stk)
{
    int error = 0;

    if ( !stk )                                                         error |= STACK_VERIFY_NULL_PNT;

    if ( stk && !(stk->data) && (stk->size != 0 || stk->capacity != 0)) error |= STACK_VERIFY_DATA_PNT_WRONG;

    if ( stk && (stk->size < 0 || stk->size > (stk->capacity) ) )       error |= STACK_VERIFY_SIZE_INVALID;

    if ( stk && stk->capacity < 0)                                      error |= STACK_VERIFY_CAPACITY_INVALID;

    return error;
}

StackErrorCode stack_ctor(Stack *stk, Elem_t *poison_value_pnt)
{
    if (!stk) return STACK_ERROR_NULL_STK_PNT_PASSED;

    stk->data = NULL;
    stk->capacity = 0;
    stk->size = 0;
    stk->poison_value_pnt = poison_value_pnt;

    return STACK_ERROR_NO_ERROR;
}

StackErrorCode stack_dtor(Stack *stk)
{
    if (!stk) return STACK_ERROR_NULL_STK_PNT_PASSED;

    stk->capacity = -1;
    stk->size = -1;
    stk->poison_value_pnt = NULL;
    if (stk->data) free(stk->data);

    return STACK_ERROR_NO_ERROR;
}

StackErrorCode stack_push(Stack *stk, Elem_t value)
{
    int verify_res = stack_verify(stk);
    if ( verify_res != 0 ) {
        STACK_DUMP(stk, verify_res);
        return STACK_ERROR_VERIFY;
    }

    StackErrorCode mem_realloc_res = stack_realloc(stk); // сам stack_realloc определяет, нужно ли делать realloc
    if ( !mem_realloc_res ) // так читаемее? так хуже потому что "нет ошибок" не всегда ноль?
    {
        return mem_realloc_res;
    }

    // нужны ли скобки ниже?
    (stk->data)[stk->size++] = value;

    return STACK_ERROR_NO_ERROR;
}

StackErrorCode stack_pop(Stack *stk, Elem_t *ret_value)
{
    int verify_res = stack_verify(stk);
    if ( verify_res != 0 ) {
        STACK_DUMP(stk, verify_res);
        return STACK_ERROR_VERIFY;
    }
    if ( !ret_value ) return STACK_ERROR_NULL_RET_VALUE_PNT;

    *ret_value = stk->data[--(stk->size)];

    if ( stk->poison_value_pnt ) stk->data[stk->size] = *(stk->poison_value_pnt);

    return STACK_ERROR_NO_ERROR;
}

//-------------------------------------------------------------------------------------------------------

//! @brief Doubles (if MEM_MULTIPLIER == 2) the capacity of the stack, allocates new memory,
//! moves data to the new place, frees old memory. Supports case when data pointer
//! equals NULL and capacity == 0.
inline StackErrorCode stack_realloc_up__( Stack *stk, const int MEM_MULTIPLIER )
{
    Elem_t *new_data = (Elem_t *) calloc( (size_t) MEM_MULTIPLIER*((size_t) stk->capacity), sizeof(Elem_t) );
    if (!new_data) return STACK_ERROR_MEM_BAD_REALLOC;

    if (stk->size > 0) if (stk->data) memcpy(new_data, stk->data, (size_t) stk->size);

    if (stk->data) free(stk->data);

    stk->data = new_data;
    stk->capacity = MEM_MULTIPLIER * stk->capacity;

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
    int verify_res = stack_verify(stk);
    if ( verify_res != 0 ) {
        STACK_DUMP(stk, verify_res);
        return STACK_ERROR_VERIFY;
    }

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

inline void stack_dump_verify_res__( int verify_res )
{
    fprintf(stderr, "Stack verification result: TEMPORARY LIKE THIS <%d>", verify_res);
}

inline void stack_dump_data__( Stack *stk )
{
    //const size_t N_FREE_SLOTS_TO_PRINT = 5;

    fprintf(stderr, "\t{\n");

    for (size_t ind = 0; ind < stk->capacity; ind++)
    {
        if ( stk->poison_value_pnt && stk->data[ind] == *(stk->poison_value_pnt) )
        {
            fprintf(stderr, "\t\t[%u]\t = <POISON VALUE>", ind);
        }
        else
        {
            fprintf(stderr, "\t\t[%u]\t = <" ELEM_T_SPECF ">", ind, stk->data[ind]);
        }

        if (stk->size == ind) fprintf(stderr, " <--");

        fprintf(stderr, "\n");
    }

    fprintf(stderr, "\t}\n");
}

void stack_dump__(Stack *stk, int verify_res, const char *file, const int line, const char *stack_name)
{
    //TODO печать в log файл, а не в stderr
    fprintf(stderr, "STACK DUMP LOG\n"); // TODO добавить временную метку?

    fprintf(stderr, "Stack[%p] \"%s\" from ... "
                    "called from %s, on line %d.", stk, stack_name, file, line);

    stack_dump_verify_res__(verify_res);

    if (!stk)
    {
        fprintf(stderr, "Stack pointer is NULL, no further information is accessible.\n");
        return;
    }

    fprintf(stderr,  "{\n"
                        "\tsize = <%ld>\n"
                        "\tcapacity = <%ld>\n"
                        "\tdata[%p]", stk->size, stk->capacity, stk->data );

    if ( !(stk->data) )
    {
        fprintf(stderr, "Data pointer is NULL. Data cannot be accessed.\n");
        return;
    }

    stack_dump_data__(stk);

    fprintf(stderr, "}\n");
}

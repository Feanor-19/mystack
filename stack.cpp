#ifndef STACK_CPP
#define STACK_CPP

#include "stack.h" // не должно ничего делать, когда этот файл включается в stack.h ?

#include <assert.h>
#include <memory.h>
#include <stdio.h>

static int stack_verify(const Stack *stk)
{
    int error = 0;

    if ( !stk )                                                         error |= STACK_VERIFY_NULL_PNT;

    if ( stk && !(stk->data) && (stk->size != 0 || stk->capacity != 0)) error |= STACK_VERIFY_DATA_PNT_WRONG;

    if ( stk && (stk->size < 0 || stk->size > (stk->capacity) ) )       error |= STACK_VERIFY_SIZE_INVALID;

    if ( stk && stk->capacity < 0)                                      error |= STACK_VERIFY_CAPACITY_INVALID;

    return error;
}

#define stack_ctor(stk) stack_ctor_(stk, #stk, __FILE__, __LINE__, __func__)

static StackErrorCode stack_ctor_( Stack *stk,
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

static StackErrorCode stack_dtor(Stack *stk)
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

static StackErrorCode stack_push(Stack *stk, Elem_t value)
{
    // ВЫНЕСТИ В ДЕФАЙН!!!
    int verify_res = stack_verify(stk);
    if ( verify_res != 0 ) {
        STACK_DUMP(stk, verify_res);
        return STACK_ERROR_VERIFY;
    }

    StackErrorCode mem_realloc_res = stack_realloc(stk); // сам stack_realloc определяет, нужно ли делать realloc
    if ( !mem_realloc_res )
    {
        return mem_realloc_res;
    }

    // нужны ли скобки ниже?
    (stk->data)[stk->size++] = value;

    return STACK_ERROR_NO_ERROR;
}

static StackErrorCode stack_pop(Stack *stk, Elem_t *ret_value)
{
    int verify_res = stack_verify(stk);
    if ( verify_res != 0 ) {
        STACK_DUMP(stk, verify_res);
        return STACK_ERROR_VERIFY;
    }
    if ( !ret_value ) return STACK_ERROR_NULL_RET_VALUE_PNT;

    *ret_value = stk->data[--(stk->size)];

    //if ( stk->poison_value_pnt ) stk->data[stk->size] = *(stk->poison_value_pnt); !!!!!!!!!!!!!!!!!!!!!!!!!!

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

static StackErrorCode stack_realloc(Stack *stk)
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

#define STACK_DO_DUMP //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1111

#ifdef STACK_DO_DUMP

inline void stack_dump_verify_res_( int verify_res )
{
    //-------------------------------------------TODO-----------------------------------------
    fprintf(stderr, "Stack verification result: TEMPORARY LIKE THIS <%d>", verify_res);
}

inline void stack_dump_data_( Stack *stk )
{
    //const size_t N_FREE_SLOTS_TO_PRINT = 5; //TODO

    fprintf(stderr, "\t{\n");

    for (long int ind = 0; ind < stk->capacity; ind++)
    {
        //if ( stk->poison_value_pnt && stk->data[ind] == *(stk->poison_value_pnt) )
        //{
        //    fprintf(stderr, "\t\t[%ld]\t = <POISON VALUE>", ind);
        //}
        //else
        //{
            fprintf(stderr, "\t\t[%ld]\t = <" ELEM_T_SPECF ">", ind, stk->data[ind]);
        //}

        if (stk->size == ind) fprintf(stderr, " <--");

        fprintf(stderr, "\n");
    }

    fprintf(stderr, "\t}\n");
}

static void stack_dump_(Stack *stk, int verify_res, const char *file, const int line, const char *stack_name)
{
    //TODO печать в log файл, а не в stderr
    fprintf(stderr, "STACK DUMP LOG\n"); // TODO добавить временнУю метку?

    fprintf(stderr, "Stack[%p] \"%s\" from ... "
                    "called from %s, on line %d.", stk, stack_name, file, line);

    stack_dump_verify_res_(verify_res);

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

    stack_dump_data_(stk);

    fprintf(stderr, "}\n");
}

#endif // STACK_DO_DUMP

#endif // STACK_CPP

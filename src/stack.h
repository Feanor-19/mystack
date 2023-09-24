#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

//ВРЕМЕННО, по хорошему это должно быть перед include stack.h
//typedef int Elem_t;
//#define ELEM_T_SPECF "%d"

//--------------------------------------------------------------------------------------------

const unsigned char POISON_VALUE = (unsigned char) 0xAA;

/*
    ------------------------------------TODO--------------------------------------
    1) все функции должны быть перенесены в .h, например с помощью include .cpp в конце .h
    а также объявлены static (возможно только делкарация, возможно и реализация, возможно всё)

    2) poison value должен быть hex константой внутри .h (размера char?), снаружи не задается, а в data
    записывается с помощью копирования этого char замощением по всему элементу, который нужно залить

    3) всё под условную компиляцию!! и под разную!! и не использовать NDEBUG!!

    4) TODO разбросанные по коду

    5) в stack_dump считаем, что всё что >= size && < capacity является poison, а рядом печатаем
    что там на самом деле
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
    long int size = -1;
    long int capacity = -1;

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

#ifdef STACK_DO_DUMP

#define STACK_DUMP(stk, verify_res) (void(0))

#else  //STACK_DO_DUMP is turned on

#define STACK_DUMP(stk, verify_res) stack_dump_( (stk), verify_res, __FILE__, __LINE__, #stk)

static void stack_dump_(Stack *stk, int verify_res, const char *file, int line, const char *stack_name);

#endif //STACK_DO_DUMP

//--------------------------------------------------------------------------------------

#include "stack.cpp"

#endif // STACK_H

#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

//ВРЕМЕННО, по хорошему это должно быть перед include stack.h
typedef int Elem_t;
#define ELEM_T_SPECF "%d"

//const long long int DEFAULT_CAPACITY = 1;

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
    long long int size = -1;
    long long int capacity = -1;

    Elem_t *poison_value_pnt = NULL; //if NULL, no poison value is used
};

//! @brief Checks stack's condition.
//! @param [in] stk Stack to check.
//! @return Mask composed from StackVerifyResFlag enum values, equaling 0 if the stack is fine.
int stack_verify(const Stack *stk);

//! @brief Stack constructor.
//! @details It doesn't allocate memory, setting size and capacity equalling 0. But
//! first push() will lead to realloc_up().
//! @param [in] stk Pointer to stack to construct.
//! @return StackErrorCode enum value.
StackErrorCode stack_ctor(Stack *stk, Elem_t *poison_value_pnt = NULL);

//! @brief Stack deconstructor.
//! @param [in] stk Pointer to stack to deconstruct.
//! @return StackErrorCode enum value.
StackErrorCode stack_dtor(Stack *stk);

//! @brief Pushes element to stack.
//! @param [in] stk Pointer to the stack.
//! @param [in] value Value to push to the stack.
//! @return StackErrorCode enum value.
StackErrorCode stack_push(Stack *stk, Elem_t value);

//! @brief Pops element from stack.
//! @param [in] stk Pointer to the stack.
//! @param [in] ret_value Pointer to put popped value to.
//! @return StackErrorCode enum value.
StackErrorCode stack_pop(Stack *stk, Elem_t *ret_value);

//! @brief Checks stack's state and, if needed, reallocs memory for the stack and changes stk->data.
//! @param [in] stk Pointer to the stack.
//! @return StackErrorCode enum value.
StackErrorCode stack_realloc(Stack *stk);

#ifdef NDEBUG

#define STACK_DUMP(stk) (void(0)) // ВОЗМОЖНО СТОИТ ПОМЕНЯТЬ НА ЧТО-ТО ДРУГОЕ

#else  //DEBUG is turned on

#define STACK_DUMP(stk, verify_res) stack_dump__( (stk), verify_res, __FILE__, __LINE__, #stk)

void stack_dump__(Stack *stk, int verify_res, const char *file, int line, const char *stack_name);

#endif //NDEBUG

#endif

#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include <greatest.h>

#include <ds/board.h>

void construct_istack() {
}

void destruct_istack() {
}

void __attribute__((constructor)) construct_6x4();
void __attribute__((destructor)) destruct_6x4();

TEST init_stack_is_empty(void) {
    IStack stack = init_istack();
    ASSERTm(
        "EXPECTED STACK WITH ZERO ELEMENTS/EMPTY",
        is_empty_istack(stack)
    );
    PASS();
}

TEST init_stack_top_array_pointer_null(void) {
    IStack stack = init_istack();
    ASSERT_FALSEm(
        "EXPECTED TOP ARRAY POINTER IN STACK TO BE NULL",
        stack.top
    );
    PASS();
}

TEST push_one_index_to_empty_stack_expected_count_one(void) {
    IStack stack = init_istack();
    push_istack(&stack, 42);
    ASSERT_EQm(
        "EXPECTED COUNT TO BE ONE",
        1, stack.count
    );
    free_istack(&stack);

    PASS();
}

TEST push_one_index_to_stack_is_empty_false(void) {
    IStack stack = init_istack();
    push_istack(&stack, 42);
    ASSERT_FALSEm(
        "EXPECTED STACK TO NOT BE EMPTY",
        is_empty_istack(stack)
    );
    free_istack(&stack);

    PASS();
}

TEST push_one_index_to_stack_top_array_not_null(void) {
    IStack stack = init_istack();
    push_istack(&stack, 42);
    ASSERTm(
        "EXPECTED TOP ARRAY POINTER TO NOT BE NULL",
        stack.top
    );
    free_istack(&stack);

    PASS();
}

TEST push_and_pop_one_index_to_empty_stack_expected_count_zero(void) {
    IStack stack = init_istack();
    push_istack(&stack, 42);
    pop_istack(&stack);
    ASSERT_EQm(
        "EXPECTED COUNT TO BE ZERO",
        0, stack.count
    );
    free_istack(&stack);

    PASS();
}

TEST push_and_pop_one_index_to_empty_stack_expected_value_42(void) {
    IStack stack = init_istack();
    ksize_t expected = 42;
    push_istack(&stack, expected);
    ksize_t actual = pop_istack(&stack);
    
    ASSERT_EQm(
        "EXPECTED VALUE TO BE 42",
        expected, actual
    );
    free_istack(&stack);
    
    PASS();
}

TEST push_and_pop_one_index_from_stack_expect_top_null(void) {
    IStack stack = init_istack();
    push_istack(&stack, 42);
    pop_istack(&stack);

    ASSERT_FALSEm(
        "EXPECTED TOP POINTER TO BE NULL",
        stack.top
    );
    free_istack(&stack);
    
    PASS();
}

TEST push_42_and_0_and_pop_from_stack_to_get_0(void) {
    IStack stack = init_istack();
    ksize_t expected = 0;

    push_istack(&stack, 42);
    push_istack(&stack, expected);

    ksize_t actual = pop_istack(&stack);
    ASSERT_EQm(
        "EXPECTED VALUE TO BE ZERO",
        expected, actual
    );
    free_istack(&stack);
    
    PASS();
}

SUITE (test_board_istack) {
    RUN_TEST(init_stack_is_empty);
    RUN_TEST(init_stack_top_array_pointer_null);
    RUN_TEST(push_one_index_to_empty_stack_expected_count_one);
    RUN_TEST(push_one_index_to_stack_is_empty_false);
    RUN_TEST(push_one_index_to_stack_top_array_not_null);
    RUN_TEST(push_and_pop_one_index_to_empty_stack_expected_count_zero);
    RUN_TEST(push_and_pop_one_index_to_empty_stack_expected_value_42);
    RUN_TEST(push_and_pop_one_index_from_stack_expect_top_null);
    RUN_TEST(push_42_and_0_and_pop_from_stack_to_get_0);
}

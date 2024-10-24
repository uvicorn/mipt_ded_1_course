#include <gtest/gtest.h>
#include "linked_list_stack.h"

class TestCout : public std::stringstream
{
public:
    ~TestCout()
    {
        std::cout << "\u001b[32m[          ] \u001b[33m" << str() << "\u001b[0m" << std::flush;
    }
};

#define TEST_COUT  TestCout()

TEST(LLStack, test_push_pop){
    LLStack* stack = LLStack_new();
    int runs = 10;
    for (stack_element i = 0; i < runs; i++){
        StackErr err = LLStack_push(stack, &i);
        ASSERT_EQ(err, NoError);
    }
    for (stack_element i = runs; i > 0; i--){
        stack_element poped = -1;
        /* TEST_COUT << "aboba=" << rui << '\n' << std::endl; */ 
        StackErr err = LLStack_pop(stack, &poped);
        ASSERT_EQ(err, NoError);
        ASSERT_EQ(poped, i-1);
    }
    stack = LLStack_delete(stack);
}

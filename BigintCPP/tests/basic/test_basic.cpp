#include <cstddef>
#include <gtest/gtest.h>
#include "basic/BigInt.hpp"
#include <ostream>

namespace BigIntBasic{


class BigIntTester : public BigInt {
    public:
        size_t blocks_count;
        BigInt::Blocks blocks;//(new UInt[new_blocks_count]);
        void Normalize();

};

std::ostream& operator<<(std::ostream& os, const BigIntTester& num) {
    os << "BigInt{blocks= [";
    for (size_t i=0; i< num.blocks_count;i++)
        os << num.blocks[i] << ", ";
    os << "], ";
    os << "blocks_count= " << num.blocks_count << ", ";
    os << "sign= " << (num.sign == PLUS ? "PLUS" : "MINUS");
    os << "}\n";
    return os;
}

class BasicBigIntFixture: public testing::Test {
    protected:
        #include "../codegen_output/basic_fixture.cpp"
        void SetUp() {
        }
        void TearDown(){
        }
};


TEST_F(BasicBigIntFixture, TestAdd){
    for (size_t index = 0; index < tests_size; index++){
        BigIntTester first = First_args[index];
        BigIntTester second = Second_args[index];
        BigIntTester res = first + second;
        EXPECT_TRUE(res == Add_res[index]) << "expected result: " << Add_res[index] << "Actual result: " << res;
    }
}

TEST_F(BasicBigIntFixture, TestSub){
    for (size_t index = 0; index < tests_size; index++){
        BigIntTester first = First_args[index];
        BigIntTester second = Second_args[index];
        BigIntTester res = first - second;
        EXPECT_TRUE(res == Sub_res[index]) << "failed on " << index;
    }
}


}

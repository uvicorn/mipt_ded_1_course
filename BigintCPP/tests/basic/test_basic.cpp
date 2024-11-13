#include <cstddef>
#include <gtest/gtest.h>
#include "basic/BigInt.hpp"
#include <ostream>
#include <tuple>
#include <utility>

namespace BigIntBasicTests{


class BigIntTester : public BigInt {
    public:
        /* BigIntTester(BigIntTester&&) = default; */
        /* size_t blocks_count; */
        /* BigInt::Blocks blocks;//(new UInt[new_blocks_count]); */
        /* void Normalize(); */
        BigIntTester(std::initializer_list<UInt> blocks, SIGN sign): BigInt(blocks, sign) {}
        BigIntTester(const BigInt& other) : BigInt(other) {}
        BigIntTester(BigInt&& other) : BigInt(std::move(other)) {}
        friend std::ostream& operator<<(std::ostream& os, const BigIntTester& num);
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
        BigIntTester& first = First_args[index];
        BigIntTester& second = Second_args[index];
        BigIntTester res = first + second;

        EXPECT_TRUE(res == Add_res[index]) 
            << "FAILED ON TEST " << index << '\n'
            << "[first arg:] " << first
            << "[second arg:] " << second
            << "expected result: " << Add_res[index]
            << "Actual result: " << res;
    }
}

TEST_F(BasicBigIntFixture, TestSub){
    for (size_t index = 0; index < tests_size; index++){
        BigIntTester& first = First_args[index];
        BigIntTester& second = Second_args[index];
        BigIntTester res = first - second;

        EXPECT_TRUE(res == Sub_res[index])
            << "FAILED ON TEST " << index << '\n'
            << "[first arg:] " << first
            << "[second arg:] " << second
            << "expected result: " << Sub_res[index]
            << "Actual result: " << res
            << '\n';
    }
}

TEST_F(BasicBigIntFixture, TestMul){
    for (size_t index = 0; index < tests_size; index++){
        BigIntTester& first = First_args[index];
        BigIntTester& second = Second_args[index];
        BigIntTester res = first * second;

        EXPECT_TRUE(res == Mul_res[index])
            << "FAILED ON TEST " << index << '\n'
            << "[first arg:] " << first
            << "[second arg:] " << second
            << "expected result: " << Mul_res[index]
            << "Actual result: " << res
            << '\n';
    }
}

TEST(BasicBigInt, TestCtors){
    BigIntTester b1 = BigInt({1,2,3,4,5}, PLUS);
    UInt arr[] = {1,2,3,4,5,6};
    BigIntTester b2 = BigInt(arr, 5, PLUS);
    EXPECT_EQ(b1, b2);

    auto c1 = BigIntTester({18446744073709551615ull, 18446744073709551615ull, 18446744073709551615ull, 18446744073709551615ull, 18446744073709551615ull, 18446744073709551615ull, 18446744073709551615ull, 18446744073709551615ull, 18446744073709551615ull, 18446744073709551615ull, 18446744073709551615ull, 18446744073709551615ull, 18446744073709551615ull, 18446744073709551615ull, 18446744073709551615ull, 1099511627775ull}, MINUS);
    auto c2 = BigInt({18446744073709551615ull, 18446744073709551615ull, 18446744073709551615ull, 18446744073709551615ull, 18446744073709551615ull, 18446744073709551615ull, 18446744073709551615ull, 18446744073709551615ull, 18446744073709551615ull, 18446744073709551615ull, 18446744073709551615ull, 18446744073709551615ull, 18446744073709551615ull, 18446744073709551615ull, 18446744073709551615ull, 1099511627775ull}, MINUS);
    auto c3 = BigIntTester(c2);

    std::cerr << "EBLOID " << c1 << "\n";
    std::cerr << "EBLOID2 " << c3 << "\n";
    EXPECT_EQ(c1, c3);
}


TEST(BasicBigInt, TestAbsLe){
    EXPECT_TRUE(BigIntTester::AbsLt(
        BigIntTester({1,2,3,4}, PLUS),
        BigIntTester({0,0,0,0,1}, MINUS)
    ));
    EXPECT_TRUE(BigIntTester::AbsLt(
        BigIntTester({1,2,3,4,0,0,0}, PLUS),
        BigIntTester({0,0,0,0,1}, MINUS)
    ));
    EXPECT_FALSE(BigIntTester::AbsLt(
        BigIntTester({1,2,3,4,0,0,0}, PLUS),
        BigIntTester({1,2,3,4}, MINUS)
    ));
    EXPECT_FALSE(BigIntTester::AbsLt(
        BigIntTester({}, PLUS),
        BigIntTester({}, MINUS)
    ));
    EXPECT_FALSE(BigIntTester::AbsLt(
        BigIntTester({}, PLUS),
        BigIntTester({}, MINUS)
    ));
}

}

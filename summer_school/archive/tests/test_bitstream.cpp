#include <cstdint>
#include <gtest/gtest.h>
#include "bitstream.h"

struct test_element {
    bs_element element;
    unsigned int bit_length;
};
struct test_array {
    unsigned int array_size;
    struct test_element elements[5];
    unsigned int sum_bit_length;
};


TEST(TestBitstream, test_write_read_single) {
    struct bitstream bs;
    bitstream_create(5000, &bs);
    struct test_element arr[] = {{1,1+3},{213213,18},{11223112312,34},{18446743950397320385ull,64},
        {323, 20}, {3,3}, {7,4}, {16,7}, {31,6}, {64,9}, {128,8}, {256,11}, {512,10}, {1023,13}, {2047,14}, {4095,14}, {8192,16}, {16384,17}, {32767,17}, {65535,18}, {131071,20}, {262144,21}, {524288,20}, {1048576,23}, {2097152,24}, {4194303,23}, {8388608,25}, {16777216,25}, {33554431,28}, {67108864,27}, {134217727,28}, {268435456,31}, {536870911,30}, {1073741823,31}, {2147483648,34}, {4294967296,34}, {8589934592,34}, {17179869183,35}, {34359738367,36}, {68719476735,37}, {137438953472,39}, {274877906943,40}, {549755813888,41}, {1099511627776,41}, {2199023255552,44}, {4398046511104,43}, {8796093022207,44}, {17592186044415,46}, {35184372088831,46}, {70368744177663,47}, {140737488355327,48}, {281474976710655,51}, {562949953421311,52}, {1125899906842623,52}, {2251799813685247,54}, {4503599627370495,55}, {9007199254740992,54}, {18014398509481983,57}, {36028797018963968,58}, {72057594037927936,59}, {144115188075855871,58}, {288230376151711743,60}, {576460752303423487,62}, {11067062460559541578ull, 64}, {13886708866595100618ull, 64}, {1838278, 21}, {14275396176336228893ull, 64}, {13510636299255178490ull, 64}
    };
    for (int test_num = 0; test_num < sizeof(arr) / sizeof(arr[0]); test_num++){
        bitstream_write_single(&bs, arr[test_num].bit_length, arr[test_num].element);
    }

    unsigned int bs_read_ptr = 0;
    for (int test_num = 0; test_num < sizeof(arr) / sizeof(arr[0]); test_num++){
        bs_element read_element = bitstream_read_single(&bs, arr[test_num].bit_length, &bs_read_ptr);
        ASSERT_EQ(arr[test_num].element, read_element);
    }
    bitstream_delete(&bs);
}
TEST(TestBitstream, test_write_64_single) {
    struct bitstream bs;
    bitstream_create(5000, &bs);
    struct test_element arr2[] = {{12909470595196007622ull, 64}, {11470691789226817512ull, 64}, {123123ull, 32}};

    for (int test_num = 0; test_num < sizeof(arr2) / sizeof(arr2[0]); test_num++){
        bitstream_write_single(&bs, arr2[test_num].bit_length, arr2[test_num].element);
    }
    unsigned int bs_read_ptr = 0;
    for (int test_num = 0; test_num < sizeof(arr2) / sizeof(arr2[0]); test_num++){
        bs_element read_element = bitstream_read_single(&bs, arr2[test_num].bit_length, &bs_read_ptr);
        ASSERT_EQ(arr2[test_num].element, read_element);
    }
    bitstream_delete(&bs);
}

TEST(TestBitstream, test_write_many) {
    struct bitstream bs;
    bitstream_create(10000, &bs);
    const int test_count = 4;
    struct test_array test_assets[test_count] = {
        {3, {{12909470595196007622ull, 64}, {11470691789226817512ull, 64}, {123123ull, 32}}},
        {4, {{11774679901490916282ull, 64}, {10587291199188076106ull, 64}, {11339886006545884107ull, 64}, {9657942389324958026ull, 64}}},
        {1, {{2222143041ull, 32}}},
        {4, {{11774679901490916282ull, 64}, {10587291199188076106ull, 64}, {11339886006545884107ull, 64}, {9657942389324958026ull, 64}}}
    };
    for (int test_index = 0; test_index < test_count; test_index++){
        struct test_array test_arr = test_assets[test_index];
        unsigned int sum_bit_length = 0;
        bs_element buf[test_arr.array_size];
        for (int i = 0; i < test_arr.array_size; i++){
            struct test_element test_element = test_arr.elements[i];
            sum_bit_length += test_element.bit_length;
            buf[i] = test_element.element;
        }
        bitstream_write_many(&bs, sum_bit_length, buf);
    }

    unsigned int bs_read_ptr = 0;
    for (int test_index = 0; test_index < test_count; test_index++){
        struct test_array test_arr = test_assets[test_index];
        for (int i = 0; i < test_arr.array_size; i++){
            struct test_element test_element = test_arr.elements[i];
            bs_element read_element = bitstream_read_single(&bs, test_element.bit_length, &bs_read_ptr);
            ASSERT_EQ(test_element.element, read_element);
        }
    }

    bitstream_delete(&bs);
}
TEST(TestBitstream, test_read_many) {
    struct bitstream bs;
    bitstream_create(10000, &bs);
    const int test_count = 4;
    struct test_array test_assets[test_count] = {
        {3, {{12909470595196007622ull, 64}, {11470691789226817512ull, 64}, {123123ull, 32}}},
        {4, {{11774679901490916282ull, 64}, {10587291199188076106ull, 64}, {11339886006545884107ull, 64}, {9657942389324958026ull, 64}}},
        {1, {{2222143041ull, 32}}},
        {4, {{11774679901490916282ull, 64}, {10587291199188076106ull, 64}, {11339886006545884107ull, 64}, {9657942389324958026ull, 64}}}
    };
    for (int test_index = 0; test_index < test_count; test_index++){
        struct test_array test_arr = test_assets[test_index];
        unsigned int sum_bit_length = 0;
        bs_element buf[test_arr.array_size];
        for (int i = 0; i < test_arr.array_size; i++){
            struct test_element test_element = test_arr.elements[i];
            sum_bit_length += test_element.bit_length;
            buf[i] = test_element.element;
        }
        test_arr.sum_bit_length = sum_bit_length;
        bitstream_write_many(&bs, sum_bit_length, buf);
    }

    unsigned int bs_read_ptr = 0;
    for (int test_index = 0; test_index < test_count; test_index++){
        struct test_array test_arr = test_assets[test_index];
        bs_element mybuf[test_arr.array_size];
        bitstream_read_many(&bs, test_arr.sum_bit_length, mybuf, &bs_read_ptr);
        for (int i = 0; i < test_arr.array_size; i++){
            struct test_element test_element = test_arr.elements[i];
            bs_element read_element = mybuf[i];
            ASSERT_EQ(test_element.element, read_element);
        }
    }

    bitstream_delete(&bs);
}

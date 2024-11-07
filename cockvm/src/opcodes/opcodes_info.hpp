#ifndef OPCODES_INFO_HPP
#define OPCODES_INFO_HPP

#include "opcodes.hpp"
#include <cstdint>

#define MAX_ARGS_COUNT 4

typedef uint32_t MemoryAddress; // TODO: вынести в другое место, так как это подвязано к стеку вм

#pragma pack(push, 1)
typedef struct OpcodeInfo {
    OpCode opcode: 8;
    uint8_t args_info: 8;
} OpcodeInfo;
#pragma pack(pop)

// ARGUMENT BIT DIMENSION
typedef enum : uint8_t {
    X64 = 0b11,
    X32 = 0b10,
    X16 = 0b01,
    X8  = 0b00,
} BIT_DIMENSION;

// REGISTER ARGUMENT

#pragma pack(push, 1)
typedef struct{
    uint8_t num: 3;
    uint8_t part: 2; // 8, 16, 32, 64
} RegId;
#pragma pack(pop)

// IMM ARGUMENT
#pragma pack(push, 1)
typedef struct {
    BIT_DIMENSION dim; // 64, 32, 16, 8 // 8 4 2 1
    uint64_t value;
} Imm;
#pragma pack(pop)


// MEMORY ARGUMENT
typedef enum: uint8_t {
    MAT_REG = 0b00,
    MAT_IMM = 0b01,
    MAT_IMM_REG = 0b10,
    MAT_IMM_REG1_REG2 = 0b11
} MemoryArgumentType;

#pragma pack(push, 1)
typedef struct {
    BIT_DIMENSION dim: 2;
    MemoryAddress address;
} MemoryArgument;
#pragma pack(pop)


#pragma pack(push, 1)
typedef struct {
    BIT_DIMENSION dim: 2; // 64, 32, 16, 8
    MemoryArgumentType type: 2;
    union {
        uint8_t reg: 5; // type: [reg]
        MemoryAddress imm; // type: [const]
        struct {
            uint8_t power: 2;
            uint8_t reg : 5;
            MemoryAddress imm_memory;
        } imm_reg;// type: [const + 2^x*reg]
        struct {
            uint8_t power: 2;
            uint8_t reg2 : 5;
            uint8_t reg1 : 5;
            MemoryAddress imm_memory;
        } imm_reg1_reg2;// type: [const + 2^x*reg1 + reg2]
    };
} Mem;
#pragma pack(pop)


// PARSER STRUCTS

typedef enum : uint8_t {
    ARG_NO       = 0b0,
    ARG_REGISTER = 0b1,
    ARG_MEMORY   = 0b10,
    ARG_IMM      = 0b11
} ARG_TYPE;

#pragma pack(push, 1)
typedef struct {
    union {
        RegId reg_id;
        Mem mem;
        Imm imm;
    };
} ArgInfo; // 80
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    ARG_TYPE arg_type; // :2
    union {
        RegId reg_id;
        MemoryArgument mem_arg;
        Imm imm;
    };
} ParsedArgInfo;
#pragma pack(pop)

typedef struct {
    uint8_t args_count = 0;
    ParsedArgInfo parsed_args[MAX_ARGS_COUNT];
} ParsedArgs;

#endif

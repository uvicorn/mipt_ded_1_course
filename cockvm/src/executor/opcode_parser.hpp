#ifndef OPCODE_PARSER_H
#define OPCODE_PARSER_H

#include "opcodes.hpp"
#include <cstdint>
#include "vm.hpp"

typedef uint32_t MemoryAddress;

typedef enum: uint8_t {
    VMPARSER_OK = 0,
    VMPARSER_INVALID_OPCODE = 1 << 0,
} VMParserError;

typedef enum : uint8_t {
    ARG_NO       = 0b0,
    ARG_REGISTER = 0b1,
    ARG_MEMORY   = 0b10,
    ARG_IMM      = 0b11
} ARG_TYPE;

typedef struct{
    uint8_t num: 3;
    uint8_t part: 2; // 8, 16, 32, 64
} RegId;

typedef enum : uint8_t {
    X64 = 0b11,
    X32 = 0b10,
    X16 = 0b01,
    X8  = 0b00,
} BIT_DIMENSION;

typedef enum: uint8_t {
    MAT_REG = 0b00,
    MAT_IMM = 0b01,
    MAT_IMM_REG = 0b10,
    MAT_IMM_REG1_REG2 = 0b11
} MemoryArgumentType;

#pragma pack(push, 1)
typedef struct {
    BIT_DIMENSION dim; // 64, 32, 16, 8 // 8 4 2 1
    uint64_t value;
} Imm;
#pragma pack(pop)

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
    ParsedArgInfo* parsed_args;
} Instruction;

VMParserError parse_arguments(VM* Vm, ParsedArgInfo* parsed_args, size_t* parser_ip);

#endif

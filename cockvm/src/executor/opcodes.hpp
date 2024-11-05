#ifndef OPCODES_H
#define OPCODES_H

#include <cstdint>

typedef enum : uint8_t {
    OP_HALT,
    OP_UD2,

    // arithmetic 32 bit
    OP_ADD,
    OP_MUL,
    OP_SUB,
    OP_DIV,

    // BITWISE ARITHMETIC
    OP_XOR,
    OP_AND,
    OP_SHR,
    OP_SHL,
    OP_NOT,

    // 
    OP_MOV,

    // stack
    OP_PUSH,
    OP_POP,

    // control flow
    OP_JMP,
    OP_JA, // TODO: define same opcodes
    OP_CALL,
    OP_RET,
    OP_CMP,

    // constants
    OP_LDC

} OpCode;

#define OPCODES_COUNT 256

#pragma pack(push, 1)
typedef struct OpcodeInfo {
    OpCode opcode: 8;
    uint8_t args_info: 8;
} OpcodeInfo;
#pragma pack(pop)

#endif

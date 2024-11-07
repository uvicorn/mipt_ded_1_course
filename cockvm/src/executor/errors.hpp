#ifndef VM_ERRORS_H
#define VM_ERRORS_H
#include "opcodes/errors.hpp"
#include "stdint.h"

typedef enum : uint32_t{
    VM_OK = 0,
    VM_INVALID_OPCODE = 1 << 0,
    VM_INVALID_OPARGS_COUNT = 1 << 1,
} VMError;

#endif

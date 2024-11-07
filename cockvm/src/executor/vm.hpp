#ifndef VM_H
#define VM_H

#include "stddef.h"
#include <cstdint>
#include "opcodes/opcodes.hpp"
#include "structs/dynamic_array.hpp"
#include "opcodes/opcodes_info.hpp"
#include "executor/errors.hpp"

/* typedef union { */
/*     uint64_t R64; */
/*     uint32_t E32; */
/*     uint16_t X16; */
/*     struct { */
/*         uint8_t L8: 8; */
/*         uint8_t H8: 8; */
/*         uint64_t __non_used: 48; */
/*     }; */
/* } RegType; */
typedef uint64_t RegType;

typedef struct {
    unsigned char* bytecode;
    size_t bytecode_size;
    RegType regs[8];
    size_t ip = 0;
    DynamicArray<MemoryAddress> address_stack;
    DynamicArray<MemoryAddress> stack;
} VM;


typedef VMError(*VMHandler)(VM *, OpcodeInfo);

#endif

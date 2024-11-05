#include "executor/opcode_parser.hpp"
#include "executor/vm.hpp"
#include "utils/compiler_opts.hpp"
#include <cassert>
#include <cstdint>


FORCE_INLINE RegType get_register_value_cut(VM* Vm, RegId reg_id){
    uint8_t shift = 64 - (1 << (3 + reg_id.part));
    uint64_t reg_value;
    /* return (Vm->regs[reg_id.num].R64 << shift) >> shift; */
    return (Vm->regs[reg_id.num] << shift) >> shift;
}

FORCE_INLINE void write_register_value(VM* Vm, RegId reg_id, uint64_t value){
    uint8_t shift = 1 << (3 + reg_id.part);
    /* Vm->regs[reg_id.num].R64 >>= shift; */
    /* Vm->regs[reg_id.num].R64 <<= shift; */
    /* Vm->regs[reg_id.num].R64 |= value; */
    Vm->regs[reg_id.num] = shift << (Vm->regs[reg_id.num] >> shift);
    Vm->regs[reg_id.num] |= value;
}
typedef enum : uint8_t{
    REGISTER,
    STACK
} WRITE_MODE;

VMError vm_read_memory(VM* Vm, ParsedArgInfo* src, uint64_t* value){
    assert(src->arg_type != ARG_IMM && src->arg_type != ARG_NO);
    switch (src->arg_type){
        case ARG_REGISTER:
            *value = get_register_value_cut(Vm, src->reg_id);
            break;
        case ARG_MEMORY: // TODO: написать стек
            break;
    }
    if (src->arg_type == ARG_REGISTER)

    return VM_OK;
}



VMError vm_write_memory(VM* Vm, ParsedArgInfo* dest, ParsedArgInfo* src){
    assert(dest->arg_type != ARG_IMM && dest->arg_type != ARG_NO);
    
    return VM_OK;
}



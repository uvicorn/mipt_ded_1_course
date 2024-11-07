#include "opcodes/deserializer.hpp"
#include "executor/vm.hpp"
#include "utils/compiler_opts.hpp"
#include <cassert>
#include <cstdint>


RegType get_register_value_part(VM* Vm, RegId reg_id){
    uint8_t shift = 64 - (1 << (3 + reg_id.part));
    uint64_t reg_value;
    /* return (Vm->regs[reg_id.num].R64 << shift) >> shift; */
    return (Vm->regs[reg_id.num] << shift) >> shift;
}

void write_register_value(VM* Vm, RegId reg_id, uint64_t value){
    uint8_t shift = 1 << (3 + reg_id.part);
    /* Vm->regs[reg_id.num].R64 >>= shift; */
    /* Vm->regs[reg_id.num].R64 <<= shift; */
    /* Vm->regs[reg_id.num].R64 |= value; */
    Vm->regs[reg_id.num] = shift << (Vm->regs[reg_id.num] >> shift);
    Vm->regs[reg_id.num] |= value;
}

VMError vm_read_memory(VM* Vm, ParsedArgInfo* src, uint64_t* value){
    /* assert(src->arg_type != ARG_IMM && src->arg_type != ARG_NO); */
    assert(src->arg_type != ARG_NO);

    switch (src->arg_type){
        case ARG_REGISTER:
            *value = get_register_value_part(Vm, src->reg_id);
            break;
        case ARG_MEMORY: // TODO: написать кучу еще
            *value = Vm->stack.get(src->mem_arg.address); // TODO: Приделать чтобы пуш на стек изменял ebp
            break;
        case ARG_IMM:
            *value = src->imm.value; // TODO: переделать с обрезанием по dim
            break;
    }
    return VM_OK;
}


VMError vm_write_memory(VM* Vm, ParsedArgInfo* dest, uint64_t value){
    assert(dest->arg_type != ARG_IMM && dest->arg_type != ARG_NO);
    switch (dest->arg_type){
        case ARG_REGISTER:
            write_register_value(Vm, dest->reg_id, value);
        case ARG_MEMORY:
            Vm->stack.get(dest->mem_arg.address) = value; // TODO: возможно стоит переписать стек без ссылок
    }
    return VM_OK;
}

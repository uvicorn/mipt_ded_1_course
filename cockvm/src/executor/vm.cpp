#include "vm.hpp"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include "utils/log.hpp"
#include "utils/compiler_opts.hpp"

VMHandler vm_handlers[OPCODES_COUNT] = {};

FORCE_INLINE VMError get_vm_handler(OpcodeInfo opcode_info, VMHandler* vm_handler){
    *vm_handler = vm_handlers[opcode_info.opcode];
    if (*vm_handler == NULL){
        return VM_INVALID_OPCODE;
    }
    return VM_OK;
}

void execute(VMContext* Vm){
    size_t IP = 0;
    while (Vm->ip < Vm->bytecode_size){
        VMHandler vm_handler = {};
        OpcodeInfo opcode_info = *reinterpret_cast<OpcodeInfo* >(&Vm->bytecode[Vm->ip]);
        VMError parse_opcode_error = get_vm_handler(opcode_info, &vm_handler);
        if (parse_opcode_error){
            LOG_ERR("INVALID OPCODE"); // TODO
        }

        VMError err = vm_handler(Vm, opcode_info);
        if (err){
            LOG_ERR("VMERROR");// TODO
        }
    }
}

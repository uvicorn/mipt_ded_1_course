#include "executor/vm.hpp"
#include "utils/compiler_opts.hpp"
#include "opcodes/deserializer.hpp"
#include "utils/log.hpp"
#include "executor/memory.hpp"

VMError OP_XOR_hanlder(VM* Vm){
    size_t current_ip= Vm->ip;
    current_ip += sizeof(OpCode);
    ParsedArgs parsed_args = {};
    VMError err = parse_arguments(Vm, &parsed_args, &current_ip); // TODO: написать обработчик ошибок
    
    uint64_t arg1 = 0;
    uint64_t arg2 = 0;

    switch (parsed_args.args_count){
        case 1:
            vm_write_memory(Vm, &parsed_args.parsed_args[0], 0ull);
            break;
        case 2:
            vm_read_memory(Vm, &parsed_args.parsed_args[0], &arg1);
            vm_read_memory(Vm, &parsed_args.parsed_args[1], &arg2);
            vm_write_memory(Vm, &parsed_args.parsed_args[0], arg1 ^ arg2);
            break;
        case 3:
            vm_read_memory(Vm, &parsed_args.parsed_args[1], &arg1);
            vm_read_memory(Vm, &parsed_args.parsed_args[2], &arg2);
            vm_write_memory(Vm, &parsed_args.parsed_args[0], arg1 ^ arg2);
            break;
    }

    return VM_OK;
}

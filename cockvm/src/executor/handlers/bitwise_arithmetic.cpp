#include "executor/vm.hpp"
#include "executor/opcodes.hpp"
#include "utils/compiler_opts.hpp"
#include "executor/opcode_parser.hpp"
#include "utils/log.hpp"

VMError OP_XOR_hanlder(VM* Vm){
    size_t current_ip= Vm->ip;
    current_ip += sizeof(OpCode);
    ParsedArgInfo parsed_args[4];
    VMParserError err = parse_arguments(Vm, parsed_args, &current_ip); // TODO: написать обработчик ошибок

}

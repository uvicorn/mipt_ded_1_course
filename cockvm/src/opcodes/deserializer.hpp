#ifndef OPCODE_PARSER_H
#define OPCODE_PARSER_H

#include "executor/vm.hpp"
#include "opcodes/opcodes_info.hpp"
#include <cstddef>




VMError parse_arguments(VM* Vm, ParsedArgs* parsed_args, size_t* parser_ip);

#endif

#include "vm.hpp"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include "opcodes.hpp"
#include "opcode_parser.hpp"

typedef int(*HandlerType)(const void *, const void *);
HandlerType arr[OPCODES_COUNT];
/* void execute(VM* Vm){ */
/*     size_t IP = 0; */
/*     while (Vm->ip < Vm->bytecode_size){ */
/*         uint8_t opcode = Vm->bytecode[Vm->ip++]; */
/*         OPARGS_COUNT opargs_count = (OPARGS_COUNT)(opcode >> 6); */
/*         uint64_t opcode_info = Vm->bytecode[Vm->ip++]; */

/*         ARG_INFO dest_info = (ARG_INFO)(opcode_info & 0b11); */
/*         ARG_INFO arg1_info = (ARG_INFO)((opcode_info>>2) & 0b11); */
/*         ARG_INFO arg2_info = (ARG_INFO)((opcode_info>>4) & 0b11); */
/*         // осталось 2 бита */
/*         uint64_t other_info = (opcode_info>>6)&0b11; */

/*         switch (opcode){ */
/*             case OP_ADD: */
/*                 break; */
/*             case OP_RET: */
/*                 IP = Vm->address_stack.pop(); */
/*                 break; */
/*             case OPARGS_NOARGS: */
/*                 switch (opcode){ */
/*                     case OP_HALT: */
/*                         return; */
/*                 } */
/*                 break; */

/*             case OPARGS_SINGLE: */
/*                 size_t dest_ptr = NULL; */
/*                 size_t */ 
/*                 switch (dest_info){ */
/*                     case ARG_MEMORY: */
/*                 } */
/*                 assert(dest_ptr != NULL); */

/*                 switch (opcode){ */
/*                     case OP_PUSH: */
/*                         Vm->stack.push(); */
/*                         break; */
                    
                    
/*                 } */
/*                 break; */
/*         } */
/*     } */
/* } */

#ifndef VM_MEMORY_H
#define VM_MEMORY_H

#include "executor/vm.hpp"
#include "opcodes/opcodes_info.hpp"
#include "utils/compiler_opts.hpp"

VMError vm_read_memory(VM* Vm, ParsedArgInfo* src, uint64_t* value);
VMError vm_write_memory(VM* Vm, ParsedArgInfo* dest, uint64_t value);

// TODO: FORCE_INLINE
RegType get_register_value_part(VM* Vm, RegId reg_id);
void write_register_value(VM* Vm, RegId reg_id, uint64_t value);



#endif

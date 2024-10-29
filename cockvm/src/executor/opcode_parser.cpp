#include "vm.hpp"
#include "opcode_parser.hpp"
#include <cstdint>
#include <variant>
#include "utils/compiler_opts.hpp"
#include "utils/log.hpp"

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


MemoryArgument parse_memory_argument(VM* Vm, Mem* mem, size_t* parser_ip){
    uint8_t reg1_id_opcode = {};
    uint8_t reg2_id_opcode = {};
    RegId reg1_id = {};
    RegId reg2_id = {};
    RegType reg1_value = {};
    RegType reg2_value = {};

    MemoryArgument memory_argument = {};
    memory_argument.dim = mem->dim;
    switch(mem->type){
        case MAT_REG:
            reg1_id_opcode = mem->reg;
            reg1_id = *reinterpret_cast<RegId* >(&reg1_id_opcode);
            reg1_value = get_register_value_cut(Vm, reg1_id);

            memory_argument.address = reg1_value;
            *parser_ip += sizeof(uint8_t) + 1;
            break;

        case MAT_IMM:
            memory_argument.address = mem->imm;
            *parser_ip += sizeof(MemoryAddress) + 1;
            break;

        case MAT_IMM_REG:
            reg1_id_opcode = mem->reg;
            reg1_id = *reinterpret_cast<RegId* >(&reg1_id_opcode);
            reg1_value = get_register_value_cut(Vm, reg1_id);

            memory_argument.address = mem->imm_reg.imm_memory + reg1_value * (1 << mem->imm_reg.power);
            *parser_ip += 6; // TODO: пофиксить константы
            break;

        case MAT_IMM_REG1_REG2:
            reg1_id_opcode = mem->imm_reg1_reg2.reg1;
            reg1_id = *reinterpret_cast<RegId* >(&reg1_id_opcode);
            reg1_value = get_register_value_cut(Vm, reg1_id);
            
            reg2_id_opcode = mem->imm_reg1_reg2.reg2;
            reg2_id = *reinterpret_cast<RegId* >(&reg2_id_opcode);
            reg2_value = get_register_value_cut(Vm, reg2_id);
            
            memory_argument.address = mem->imm_reg.imm_memory + reg1_value * (1 << mem->imm_reg.power) + reg2_value;
            *parser_ip += sizeof(MemoryAddress) + 7;
            break;

        default:
            LOG_ERR("Memory argument has invalid");
    }
    return memory_argument;
}


/* std::variant<MemoryArgument, RegId, Imm> ParseArg(){ */
/*     OpcodeInfo opcode_info = (uint8_t)(Vm->bytecode[Vm->ip]); */
/*     switch (opcode_info.opargs_count){ */
/*         case OPARGS_NOARGS: */
/*             return (NoArgsOpcode)opcode_info; */
/*         case OPARGS_SINGLE: */
/*             switch (){ */
/*                 return Opcode */
/*             } */
/*             break; */
/*     } */
/* } */



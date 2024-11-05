#include "vm.hpp"
#include "opcode_parser.hpp"
#include <cstdint>
#include <variant>
#include "utils/compiler_opts.hpp"
#include "utils/log.hpp"
#include "executor/memory.hpp"

VMParserError parse_register_argument(VM* Vm, RegId* reg_id, size_t* parser_ip, RegId* reg_argument){
    *reg_argument = *reg_id;
    *parser_ip += sizeof(RegId);
    return VMPARSER_OK;
}

VMParserError parse_imm_argument(VM* Vm, Imm* imm, size_t* parser_ip,  Imm* imm_argument){
    imm_argument->dim = imm->dim;
    uint8_t imm_byte_size = 1 << imm->dim;
    uint8_t imm_bit_size = (8*imm_byte_size);
    imm_argument->value = (imm->value << imm_bit_size) >> imm_byte_size;
    *parser_ip += imm_byte_size;
    return VMPARSER_OK;
}
VMParserError parse_memory_argument(VM* Vm, Mem* mem, size_t* parser_ip,  MemoryArgument* memory_argument){
    uint8_t reg1_id_opcode = {};
    uint8_t reg2_id_opcode = {};
    RegId reg1_id = {};
    RegId reg2_id = {};
    RegType reg1_value = {};
    RegType reg2_value = {};

    memory_argument->dim = mem->dim;
    switch(mem->type){
        case MAT_REG:
            reg1_id_opcode = mem->reg;
            reg1_id = *reinterpret_cast<RegId* >(&reg1_id_opcode);
            reg1_value = get_register_value_cut(Vm, reg1_id);

            memory_argument->address = reg1_value;
            *parser_ip += sizeof(uint8_t) + 1;
            break;

        case MAT_IMM:
            memory_argument->address = mem->imm;
            *parser_ip += sizeof(MemoryAddress) + 1;
            break;

        case MAT_IMM_REG:
            reg1_id_opcode = mem->reg;
            reg1_id = *reinterpret_cast<RegId* >(&reg1_id_opcode);
            reg1_value = get_register_value_cut(Vm, reg1_id);

            memory_argument->address = mem->imm_reg.imm_memory + reg1_value * (1 << mem->imm_reg.power);
            *parser_ip += 6; // TODO: пофиксить константы
            break;

        case MAT_IMM_REG1_REG2:
            reg1_id_opcode = mem->imm_reg1_reg2.reg1;
            reg1_id = *reinterpret_cast<RegId* >(&reg1_id_opcode);
            reg1_value = get_register_value_cut(Vm, reg1_id);
            
            reg2_id_opcode = mem->imm_reg1_reg2.reg2;
            reg2_id = *reinterpret_cast<RegId* >(&reg2_id_opcode);
            reg2_value = get_register_value_cut(Vm, reg2_id);
            
            memory_argument->address = mem->imm_reg.imm_memory + reg1_value * (1 << mem->imm_reg.power) + reg2_value;
            *parser_ip += sizeof(MemoryAddress) + 7;
            break;

        default:
            LOG_ERR("Memory argument has invalid");
            return VMPARSER_INVALID_OPCODE;
    }
}


VMParserError parse_arguments(VM* Vm, ParsedArgInfo* parsed_args, size_t* parser_ip){
    /* OpCode opcode = static_cast<OpCode>(Vm->bytecode[*parser_ip++]); */
    uint8_t args_info = Vm->bytecode[*parser_ip++];

    while (args_info){
        ARG_TYPE arg_type = static_cast<ARG_TYPE>(args_info & 0b11);
        ArgInfo not_parsed_arg_info =  *reinterpret_cast<ArgInfo* >(&Vm->bytecode[*parser_ip]);

        ParsedArgInfo parsed_arg = {};
        parsed_arg.arg_type = arg_type;
        VMParserError parser_error = VMPARSER_OK;
        
        switch (arg_type){
            case ARG_NO:
                continue;
                break;

            case ARG_IMM:
                parser_error = parse_imm_argument(Vm, &not_parsed_arg_info.imm, parser_ip, &parsed_arg.imm);
                break;

            case ARG_MEMORY:
                parser_error = parse_memory_argument(Vm, &not_parsed_arg_info.mem, parser_ip, &parsed_arg.mem_arg);
                break;

            case ARG_REGISTER:
                parser_error = parse_register_argument(Vm, &not_parsed_arg_info.reg_id, parser_ip, &parsed_arg.reg_id);
                break;
        }
        if (parser_error != VMPARSER_OK){
            return parser_error;
        }
        args_info >>= 2;
    }
    return VMPARSER_OK;
}

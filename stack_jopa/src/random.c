#include "stack.h"
#include <asm/unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <stddef.h>
#include "random.h"


// 0) https://stackoverflow.com/questions/9506353/how-to-invoke-a-system-call-via-syscall-or-sysenter-in-inline-assembly
// 1) https://wiki.osdev.org/Inline_Assembly
// 2) https://gcc.gnu.org/onlinedocs/gcc/extensions-to-the-c-language-family/how-to-use-inline-assembly-language-in-c-code.html#id2
inline ssize_t getrandom(char* buf, size_t count, unsigned int flags){
    ssize_t ret;
    asm volatile
    (
        "syscall"
        : "=a" (ret)
        :   "0"(SYS_getrandom),
            "D"(buf),           // D - RDI
            "S"(count),         // S - RSI
            "d"(flags)          // d - RDX
        : "rcx", "r11", "memory" // clobbers
    );
    return ret;
}
// МАМА Я ЛЮБЛЮ КОКАИН
// МАМА Я ЛЮБЛЮ САТАНУ

inline void generate_stack_canary(char* buf){
    getrandom(buf, sizeof(stack_element), NULL);
}

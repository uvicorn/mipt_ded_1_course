
#define __USE_GNU

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/ucontext.h>
#include "ucontext.h"
enum
{
  REG_R8 = 0,
# define REG_R8		REG_R8
  REG_R9,
# define REG_R9		REG_R9
  REG_R10,
# define REG_R10	REG_R10
  REG_R11,
# define REG_R11	REG_R11
  REG_R12,
# define REG_R12	REG_R12
  REG_R13,
# define REG_R13	REG_R13
  REG_R14,
# define REG_R14	REG_R14
  REG_R15,
# define REG_R15	REG_R15
  REG_RDI,
# define REG_RDI	REG_RDI
  REG_RSI,
# define REG_RSI	REG_RSI
  REG_RBP,
# define REG_RBP	REG_RBP
  REG_RBX,
# define REG_RBX	REG_RBX
  REG_RDX,
# define REG_RDX	REG_RDX
  REG_RAX,
# define REG_RAX	REG_RAX
  REG_RCX,
# define REG_RCX	REG_RCX
  REG_RSP,
# define REG_RSP	REG_RSP
  REG_RIP,
# define REG_RIP	REG_RIP
  REG_EFL,
# define REG_EFL	REG_EFL
  REG_CSGSFS,		/* Actually short cs, gs, fs, __pad0.  */
# define REG_CSGSFS	REG_CSGSFS
  REG_ERR,
# define REG_ERR	REG_ERR
  REG_TRAPNO,
# define REG_TRAPNO	REG_TRAPNO
  REG_OLDMASK,
# define REG_OLDMASK	REG_OLDMASK
  REG_CR2
# define REG_CR2	REG_CR2
};
static void default_segv_handler(int sig){
    if (sig == SIGSEGV) printf("Caught signal for Ctrl+C\n");
}

static void segv_handler(int signum, siginfo_t * si, void *vcontext){
    printf("exit addr:, 0x%016llx\n", si->si_addr);
    ucontext_t* context = (ucontext_t*)vcontext;
    mcontext_t mctx = context->uc_mcontext;/* context-> */
    long long int* reg_arr = mctx.gregs;
    printf("rip: 0x%016llx\n", reg_arr[REG_RIP]);
    for (int i=0;i<10;i++){
        printf("REG %d: 0x%016llx\n",i,reg_arr[i]);
    }
    exit(-1);
}

int main(){
    printf("start program");
    struct sigaction action;
    action.sa_handler = segv_handler;
    action.sa_flags = SA_SIGINFO;
    sigaction(SIGSEGV, &action, NULL);
    /* asm("xor rip,rip"); */
    int* x = 0;
    int y = *x;

    printf("end program");
}

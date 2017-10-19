#include <capstone.h>
#include <stdio.h>

extern unsigned char code;
extern unsigned char end;

int main() {
    csh handle;
    if(cs_open(CS_ARCH_X86, CS_MODE_64, &handle) != CS_ERR_OK) return 1;

    cs_option(handle, CS_OPT_SYNTAX, CS_OPT_SYNTAX_ATT);  // AT&T syntax
    cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);

    cs_insn *insn;
    size_t count = cs_disasm(handle, (void *)&code, &end - &code,
        0x1000, 10, &insn);
    printf("found %zu instructions\n", count);
    for(size_t i = 0; i < count; i ++) {
        cs_insn *ins = &insn[i];
        int n = 0;
        for(size_t b = 0; b < ins->size; b ++) {
            n += printf("%02x ", ins->bytes[b]);
        }
        printf("%*s0x%lx:  %-5s %s\n", 24-n, "",
            ins->address, ins->mnemonic, ins->op_str);

        for(int j = 0; j < ins->detail->x86.op_count; j ++) {
            printf("    arg[%d]: ", j);
            cs_x86_op *op = &ins->detail->x86.operands[j];
            switch(op->type) {
            case X86_OP_REG:
                printf("(reg %s)", cs_reg_name(handle, op->reg));
                break;
            case X86_OP_IMM:
                printf("(imm 0x%lx)", op->imm);
                break;
            case X86_OP_MEM:
                printf("(mem 0x%lx+%s+%s*%d)", op->mem.disp,
                    cs_reg_name(handle, op->mem.base),
                    cs_reg_name(handle, op->mem.index), op->mem.scale);
                break;
            /*case X86_OP_FP:
                printf("(fp %f)", op->fp);
                break;*/
            }
            printf(" size %d\n", op->size);
        }
    }

    cs_free(insn, count);
    cs_close(&handle);
    return 0;
}

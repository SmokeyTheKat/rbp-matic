#ifndef __rmvm_cpu_h__
#define __rmvm_cpu_h__

#include <string.h>

#include "./utils.h"

struct program;
struct regs;
struct cpu;

struct cpu make_cpu(void);
void cpu_load_program(struct cpu* cpu, const struct program program);
void cpu_emulate(struct cpu* cpu);
struct opc cpu_pull_value(struct cpu* cpu);

struct regs
{
	long r0, r1, r2, r3, r4, r5, r6, r7, r8, r9;
	long g0, g1, g2, g3, g4, g5, g6, g7, g8, g9;
	long f0, f1, f2, f3;
	long bp;
	long sp;
	long ip;
};

struct program
{
	struct buffer code;
	struct buffer data;
	struct buffer stack;
};

struct cpu
{
	struct regs regs;
};

struct cpu make_cpu(void)
{
	struct cpu output;
	output.regs.r0 = 0;
	output.regs.r1 = 0;
	output.regs.r2 = 0;
	output.regs.r3 = 0;
	output.regs.r4 = 0;
	output.regs.r5 = 0;
	output.regs.r6 = 0;
	output.regs.r7 = 0;
	output.regs.r8 = 0;
	output.regs.r9 = 0;

	output.regs.g0 = 0;
	output.regs.g1 = 0;
	output.regs.g2 = 0;
	output.regs.g3 = 0;
	output.regs.g4 = 0;
	output.regs.g5 = 0;
	output.regs.g6 = 0;
	output.regs.g7 = 0;
	output.regs.g8 = 0;
	output.regs.g9 = 0;

	output.regs.f0 = 0;
	output.regs.f1 = 0;
	output.regs.f2 = 0;
	output.regs.f3 = 0;

	output.regs.bp = 0;
	output.regs.sp = 0;
	output.regs.ip = 0;

	return output;
}

void cpu_load_program(struct cpu* cpu, const struct program program)
{
	byte* memspace = malloc(program.code.len +
				program.data.len +
				program.stack.len);
	memcpy(memspace + 0, program.code.data, program.code.len);
	memcpy(memspace + program.code.len, program.data.data, program.data.len);
	memcpy(memspace + program.code.len + program.data.len, program.stack.data, program.stack.len);
	cpu->regs.ip = (long)(memspace);
	cpu->regs.bp = (long)(memspace + program.code.len + program.data.len);
	cpu->regs.bp = (long)(memspace + program.code.len + program.data.len);
}

void cpu_emulate(struct cpu* cpu)
{
	bool exit = false;
	while (!exit)
	{
		byte com = *((byte*)(cpu->regs.ip++));
		switch (com)
		{
			case COM_EXIT:
			{
				exit = true;
			} break;
			case COM_MOV:
			{
				ddPrintf("mov ");
				struct opc lhs = cpu_pull_value(cpu);
				struct opc rhs = cpu_pull_value(cpu);
			} break;
			default:
				break;
		}
		ddPrint_nl();
	}
}

struct opc cpu_pull_value(struct cpu* cpu)
{
	struct opc output;
	output.type = -1;
	output.deref_as = 0;
	byte nxt = *((byte*)(cpu->regs.ip++));
	switch (nxt)
	{
		case RCODE_REGISTER:
		{
			output.type = TKN_REGISTER;
			byte reg = *((byte*)(cpu->regs.ip++));
			output.opc.reg = reg;
			ddPrintf("reg: %d, ", reg);
		} break;
		case RCODE_BYTE:
		{
			output.type = TKN_NUMBER;
			byte num = *((byte*)(cpu->regs.ip++));
			ddPrintf("num: %d, ", num);
		} break;
		case RCODE_WORD:
		{
			output.type = TKN_NUMBER;
			short num = *((short*)(cpu->regs.ip));
			cpu->regs.ip += 2;
			ddPrintf("num: %d, ", num);
			
		} break;
		case RCODE_DWORD:
		{
			output.type = TKN_NUMBER;
			int num = *((int*)(cpu->regs.ip));
			cpu->regs.ip += 4;
			ddPrintf("num: %d, ", num);
			
		} break;
		case RCODE_QWORD:
		{
			output.type = TKN_NUMBER;
			long num = *((long*)(cpu->regs.ip));
			cpu->regs.ip += 8;
			ddPrintf("num: %d, ", num);
			
		} break;
	}
	return output;
}

#endif

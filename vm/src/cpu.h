#ifndef __rmvm_cpu_h__
#define __rmvm_cpu_h__

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "./utils.h"

struct program;
struct regs;
struct cpu;

struct cpu make_cpu(void);
void cpu_load_program(struct cpu* cpu, const struct program program);
long* cpu_get_reg(struct cpu* cpu, struct opc opc);
long cpu_get_value(struct cpu* cpu, struct opc opc);
void cpu_emulate(struct cpu* cpu);
struct opc cpu_pull_value(struct cpu* cpu);
void cpu_execute_syscall(struct cpu* cpu);
void cpu_execute_mov(struct cpu* cpu, struct opc lhs, struct opc rhs);
void cpu_execute_add(struct cpu* cpu, struct opc lhs, struct opc rhs);
void cpu_execute_sub(struct cpu* cpu, struct opc lhs, struct opc rhs);
void cpu_execute_mul(struct cpu* cpu, struct opc lhs);
void cpu_execute_div(struct cpu* cpu, struct opc lhs);

struct regs
{
	long r[10];
	long g[10];
	long f[10];
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
	output.regs.r[0] = 0;
	output.regs.r[1] = 0;
	output.regs.r[2] = 0;
	output.regs.r[3] = 0;
	output.regs.r[4] = 0;
	output.regs.r[5] = 0;
	output.regs.r[6] = 0;
	output.regs.r[7] = 0;
	output.regs.r[8] = 0;
	output.regs.r[9] = 0;

	output.regs.g[0] = 0;
	output.regs.g[1] = 0;
	output.regs.g[2] = 0;
	output.regs.g[3] = 0;
	output.regs.g[4] = 0;
	output.regs.g[5] = 0;
	output.regs.g[6] = 0;
	output.regs.g[7] = 0;
	output.regs.g[8] = 0;
	output.regs.g[9] = 0;

	output.regs.f[0] = 0;
	output.regs.f[1] = 0;
	output.regs.f[2] = 0;
	output.regs.f[3] = 0;
	output.regs.f[4] = 0;
	output.regs.f[5] = 0;
	output.regs.f[6] = 0;
	output.regs.f[7] = 0;
	output.regs.f[8] = 0;
	output.regs.f[9] = 0;

	output.regs.bp = 0;
	output.regs.sp = 0;
	output.regs.ip = 0;

	return output;
}

long cpu_get_value(struct cpu* cpu, struct opc opc)
{
	switch (opc.type)
	{
		case TKN_NUMBER:
			return opc.opc.num;
			break;
		case TKN_REGISTER:
			return *(cpu_get_reg(cpu, opc));
			break;
		default:
			break;
	}
	return 0;
}
long* cpu_get_reg(struct cpu* cpu, struct opc opc)
{
	long* rl;
	int rc = opc.opc.reg % 10;
	switch (opc.opc.reg / 10)
	{
		case 0:
			rl = &(cpu->regs.r[rc]);
			break;
		case 1:
			rl = &(cpu->regs.g[rc]);
			break;
		case 2:
			rl = &(cpu->regs.f[rc]);
			break;
	}
	return rl;
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

void print_bin(byte v)
{
	while (v)
	{
		if (v & 1) ddPrintf("1");
		else ddPrintf("0");
		v >>= 1;
	}
	ddPrint_nl();
}

void print_regs(struct cpu* cpu)
{
	for (int i = 0; i < 10; i++)
		ddPrintf("r[%d] = %d\n", i, cpu->regs.r[i]);
/*
	for (int i = 0; i < 10; i++)
		ddPrintf("g[%d] = %d\n", i, cpu->regs.g[i]);
*/
	for (int i = 0; i < 10; i++)
		ddPrintf("f[%d] = %d\n", i, cpu->regs.f[i]);
}

void cpu_emulate(struct cpu* cpu)
{
	bool pexit = false;
	while (!pexit)
	{
		//print_regs(cpu);
		struct opc lhs;
		struct opc rhs;
		byte com = *((byte*)(cpu->regs.ip++));
		switch (com)
		{
			case COM_EXIT:
			{
				pexit = true;
				ddPrintf("exit\n");
			} break;
			case COM_SYSCALL:
			{
				cpu_execute_syscall(cpu);
			} break;
			case COM_MOV:
			{
				//ddPrintf("mov ");
				lhs = cpu_pull_value(cpu);
				rhs = cpu_pull_value(cpu);
				cpu_execute_mov(cpu, lhs, rhs);
			} break;
			case COM_ADD:
			{
				//ddPrintf("add ");
				lhs = cpu_pull_value(cpu);
				rhs = cpu_pull_value(cpu);
				cpu_execute_add(cpu, lhs, rhs);
			} break;
			case COM_SUB:
			{
				//ddPrintf("sub ");
				lhs = cpu_pull_value(cpu);
				rhs = cpu_pull_value(cpu);
				cpu_execute_sub(cpu, lhs, rhs);
			} break;
			case COM_MUL:
			{
				//ddPrintf("mul ");
				lhs = cpu_pull_value(cpu);
				cpu_execute_mul(cpu, lhs);
			} break;
			case COM_DIV:
			{
				//ddPrintf("dib ");
				lhs = cpu_pull_value(cpu);
				cpu_execute_div(cpu, lhs);
			} break;
			default:
				break;
		}
		//ddPrint_nl();
	}
}

void sc0(long f[10])
{
	ddPrintf("NUMBER: %d\n", f[1]);
}

const void (*cpu_syscalls[])(long f[10]) = {
	sc0,
};

void cpu_execute_syscall(struct cpu* cpu)
{
	(*(cpu_syscalls[cpu->regs.f[0]]))(cpu->regs.f);
}

void cpu_execute_mul(struct cpu* cpu, struct opc lhs)
{
	cpu->regs.f[0] *= cpu_get_value(cpu, lhs);
}
void cpu_execute_div(struct cpu* cpu, struct opc lhs)
{
	cpu->regs.f[1] = cpu->regs.f[0];
	cpu->regs.f[0] /= cpu_get_value(cpu, lhs);
	cpu->regs.f[1] %= cpu_get_value(cpu, lhs);
}
void cpu_execute_sub(struct cpu* cpu, struct opc lhs, struct opc rhs)
{
	(*(cpu_get_reg(cpu, lhs))) -= cpu_get_value(cpu, rhs);
}
void cpu_execute_add(struct cpu* cpu, struct opc lhs, struct opc rhs)
{
	(*(cpu_get_reg(cpu, lhs))) += cpu_get_value(cpu, rhs);
}
void cpu_execute_mov(struct cpu* cpu, struct opc lhs, struct opc rhs)
{
	*(cpu_get_reg(cpu, lhs)) = cpu_get_value(cpu, rhs);
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
			//ddPrintf("reg: %d, ", reg);
		} break;
		case RCODE_BYTE:
		{
			output.type = TKN_NUMBER;
			byte num = *((byte*)(cpu->regs.ip++));
			output.opc.num = num;
			//ddPrintf("num: %d, ", num);
		} break;
		case RCODE_WORD:
		{
			output.type = TKN_NUMBER;
			short num = *((short*)(cpu->regs.ip));
			output.opc.num = num;
			cpu->regs.ip += 2;
			//ddPrintf("num: %d, ", num);
			
		} break;
		case RCODE_DWORD:
		{
			output.type = TKN_NUMBER;
			int num = *((int*)(cpu->regs.ip));
			output.opc.num = num;
			cpu->regs.ip += 4;
			//ddPrintf("num: %d, ", num);
			
		} break;
		case RCODE_QWORD:
		{
			output.type = TKN_NUMBER;
			long num = *((long*)(cpu->regs.ip));
			output.opc.num = num;
			cpu->regs.ip += 8;
			//ddPrintf("num: %d, ", num);
			
		} break;
		default:
			break;
	}
	return output;
}

#endif

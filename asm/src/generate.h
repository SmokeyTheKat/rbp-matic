#ifndef __rmasm_generate_h__
#define __rmasm_generate_h__

#include "utils.h"
#include "lexer.h"

#define RCODE_REGISTER 0x21
#define RCODE_BYTE 0x11
#define RCODE_WORD 0x12
#define RCODE_DWORD 0x13
#define RCODE_QWORD 0x14

struct buffer generate_main(struct token* tokens, sizet token_count);
void emit_data(struct token* tokens, int* pos, struct buffer* buf);

struct buffer generate_main(struct token* tokens, sizet token_count)
{
	ddPrintf("sv: %d\n", labels[0].value);
	struct buffer buf;
	buf.len = 0;
	buf.data = make(byte, 100000);
	for (int i = 0; i < token_count;)
	{
		if (tokens[i].type == GEN_SLB)
		{
			labels[tokens[i++].opc.sbl].value = buf.len;
			ddPrintf("set l v: %d\n", labels[tokens[i-1].opc.sbl].value);
			continue;
		}
		if (tokens[i].type != TKN_COMMAND) compile_error("INCORRECT OPC ORDER");
		switch (tokens[i].opc.com)
		{
			case COM_MOV:
			case COM_ADD:
			case COM_SUB:
				buf.data[buf.len++] = tokens[i++].opc.com;
				emit_data(tokens, &i, &buf);
				emit_data(tokens, &i, &buf);
				break;
			case COM_MUL:
			case COM_DIV:
				buf.data[buf.len++] = tokens[i++].opc.com;
				emit_data(tokens, &i, &buf);
				break;
			case COM_SYSCALL:
			case COM_JMP:
			case COM_EXIT:
				buf.data[buf.len++] = tokens[i++].opc.com;
			default:
				break;
		}
	}
	ddPrintf("ev: %d\n", labels[0].value);
	return buf;
}

void emit_data(struct token* tokens, int* pos, struct buffer* buf)
{
	switch (tokens[*pos].type)
	{
		case GEN_LLB:
		{
			*((long*)&(buf->data[buf->len])) = (long)(labels[get_label_index(tokens[*pos].opc.lbl)].value);
			ddPrintf("NUMBRERRRR: %d\n", (long)(labels[get_label_index(tokens[*(pos)].opc.lbl)].value));
			buf->len += 8;
			(*pos)++;
		} break;
		case TKN_REGISTER:
		{
			buf->data[buf->len++] = RCODE_REGISTER;
			buf->data[buf->len++] = tokens[*(pos)].opc.reg;
			(*pos)++;
		} break;
		case TKN_NUMBER:
		{
			buf->data[buf->len++] = RCODE_QWORD;
			long value = tokens[*(pos)+1].opc.num;
			*((long*)&(buf->data[buf->len])) = *(((long*)(&value)));
			buf->len += 8;
			(*pos)++;
		} break;
		case TKN_PREFIX:
		{
			switch (tokens[*pos].opc.pfx)
			{
				case PFX_BYTE:
				{
					buf->data[buf->len++] = RCODE_BYTE;
					char value = tokens[*(pos)+1].opc.num;
					buf->data[buf->len++] = value;
					
				} break;
				case PFX_WORD:
				{
					buf->data[buf->len++] = RCODE_WORD;
					short value = tokens[*(pos)+1].opc.num;
					*((short*)&(buf->data[buf->len])) = *(((short*)(&value)));
					buf->len += 2;
				} break;
				case PFX_DWORD:
				{
					buf->data[buf->len++] = RCODE_DWORD;
					int value = tokens[*(pos)+1].opc.num;
					*((int*)&(buf->data[buf->len])) = *(((int*)(&value)));
					buf->len += 4;
				} break;
				case PFX_QWORD:
				{
					buf->data[buf->len++] = RCODE_QWORD;
					long value = tokens[*(pos)+1].opc.num;
					*((long*)&(buf->data[buf->len])) = *(((long*)(&value)));
					buf->len += 8;
				} break;
			}
			(*pos) += 2;
		} break;
		default:
			compile_error("INCORRECT OPC");
			break;
	}
}

#endif

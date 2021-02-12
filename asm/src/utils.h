#ifndef __rmasm_utils_h__
#define __rmasm_utils_h__

#define TKN_NUMBER 0x00
#define TKN_STRING 0x01
#define TKN_REGISTER 0x02
#define TKN_PREFIX 0x03
#define TKN_COMMAND 0x04

#define GEN_LLB 0x05
#define GEN_SLB 0x06

#define PFX_BYTE 0x00
#define PFX_WORD 0x01
#define PFX_DWORD 0x02
#define PFX_QWORD 0x03

#define COM_MOV 0x30
#define COM_ADD 0x31
#define COM_SUB 0x32
#define COM_MUL 0x33
#define COM_DIV 0x34
#define COM_SYSCALL 0x35
#define COM_EXIT 0x36
#define COM_JMP 0x37

typedef char byte;

struct buffer;
struct token;
struct command;
struct label;
union opc;

void write_file(const char* path, struct buffer file);
ddString read_file(const char* path);

union opc
{
	byte reg;
	long num;
	ddString str;
	long com;
	long pfx;
	ddString lbl;
	int sbl;
};
struct label
{
	ddString name;
	long value;
};
struct token
{
	int type;
	union opc opc;
};
struct command
{
	struct token* opcs[10];
	int command;
};
struct buffer
{
	byte* data;
	sizet len;
};
struct opc_id
{
	char* cstr;
	int id;
};

const int REGISTER_IDS[120] = {
	['r'] = 0,
	['g'] = 10,
	['f'] = 20,
};

const struct opc_id OPERATOR_OPC_IDS[] = {
	{ "mov", COM_MOV },
	{ "add", COM_ADD },
	{ "sub", COM_SUB },
	{ "mul", COM_MUL },
	{ "div", COM_DIV },
	{ "syscall", COM_SYSCALL },
	{ "exit", COM_EXIT },
};
const struct opc_id PREFIX_OPC_IDS[] = {
	{ "byte", PFX_BYTE },
	{ "word", PFX_WORD },
	{ "dword", PFX_DWORD },
	{ "qword", PFX_QWORD },
};

void write_file(const char* path, struct buffer file)
{
	FILE* fp = fopen(path, "w");
	if (fp == null) return;
	fwrite(file.data, sizeof(char), file.len, fp);
	fclose(fp);
}

ddString read_file(const char* path)
{
	FILE* fp = fopen(path, "r");
	if (fp == null) return make_constant_ddString("ERROR");
	fseek(fp, 0L, SEEK_END);
	long nb = ftell(fp) - 1;
	fseek(fp, 0L, SEEK_SET);
	char* buffer = (char*)calloc(nb+1, sizeof(char));
	fread(buffer, sizeof(char), nb, fp);
	fclose(fp);
	buffer[nb] = '\0';
	return make_ddString(buffer);
}

void compile_error(char* message)
{
	ddPrintf("[\x1b[38;2;255;0;0mERROR\x1b[38;2;255;255;255m] %s\n", message);
	exit(1);
}


#endif

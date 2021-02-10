#ifndef __rmvm_utils_h__
#define __rmvm_utils_h__

#include <stdlib.h>
#include <stdio.h>

#define TKN_NUMBER 0x00
#define TKN_STRING 0x01
#define TKN_REGISTER 0x02
#define TKN_PREFIX 0x03
#define TKN_COMMAND 0x04

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

#define RCODE_REGISTER 0x21
#define RCODE_BYTE 0x11
#define RCODE_WORD 0x12
#define RCODE_DWORD 0x13
#define RCODE_QWORD 0x14


typedef char byte;

struct buffer;
union type;

struct buffer read_file(const char* path);

union type
{
	int reg;
	long num;
};

struct opc
{
	union type opc;
	int type;
	int deref_as;
};

struct buffer
{
	byte* data;
	sizet len;
};

struct buffer read_file(const char* path)
{
	struct buffer output = { (byte*)nullptr, 0 };
	FILE* fp = fopen(path, "rb");
	if (fp == null) return output;
	fseek(fp, 0L, SEEK_END);
	long nb = ftell(fp) - 1;
	fseek(fp, 0L, SEEK_SET);
	byte* buffer = (byte*)calloc(nb+1, sizeof(byte));
	fread(buffer, sizeof(byte), nb, fp);
	fclose(fp);
	output.data = buffer;
	output.len = nb;
	return output;
}

#endif

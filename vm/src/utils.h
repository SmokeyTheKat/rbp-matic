#ifndef __rmvm_utils_h__
#define __rmvm_utils_h__

#include <stdlib.h>
#include <stdio.h>

typedef char byte;

struct buffer;

struct buffer read_file(const char* path);

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

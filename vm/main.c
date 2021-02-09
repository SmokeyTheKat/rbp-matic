
struct buffer
{
	byte* data;
	sizet length;
};

void write_file(const char* path, ddString content)
{
	ddString filename = make_ddString(path);
	ddString_push_cstring_back(&filename, ".s");
	filename.cstr[filename.length] = '\0';
	FILE* fp = fopen(filename.cstr, "w");
	if (fp == null) return;
	fwrite(content.cstr, sizeof(char), content.length, fp);
	fclose(fp);
	raze_ddString(&filename);
}

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
	output.length = nb;
	return output;
}
int main(void)
{
	
	return 0;
}

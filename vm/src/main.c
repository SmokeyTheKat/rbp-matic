#include <stdlib.h>
#include <stdio.h>
#include <ddcString.h>
#include <ddcPrint.h>

#include "./utils.h"
#include "./cpu.h"
#include "./args.h"

int main(int argc, char** argv)
{
	read_args(argc, argv);
	ddString file_path = args_get_value(make_constant_ddString("__INPUT_FILE"));
	struct buffer buf = read_file(file_path.cstr);
	struct cpu cpu = make_cpu();
	struct program prgm;
	prgm.code = buf;
	prgm.data.data = (byte*)(nullptr);
	prgm.data.len = 0;
	prgm.stack.len = 1000000;
	prgm.stack.data = malloc(prgm.stack.len);
	cpu_load_program(&cpu, prgm);
	cpu_emulate(&cpu);
	return 0;
}

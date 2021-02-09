#include <stdlib.h>
#include <stdio.h>
#include <ddcString.h>
#include <ddcPrint.h>

#include "./utils.h"
#include "./args.h"

int main(int argc, char** argv)
{
	read_args(argc, argv);
	ddString file_path = args_get_value(make_constant_ddString("__INPUT_FILE"));
	struct buffer buf = read_file(file_path.cstr);
	long* val = (long*)(buf.data+4);
	ddPrintf("value: %d\n", *val);
	return 0;
}

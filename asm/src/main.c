#include <stdlib.h>
#include <stdio.h>
#include <ddcDef.h>
#include <ddcString.h>
#include <ddcPrint.h>

#include "./utils.h"
#include "./args.h"
#include "./lexer.h"
#include "./generate.h"

int main(int argc, char** argv)
{
	read_args(argc, argv);
	ddString file_path = args_get_value(make_constant_ddString("__INPUT_FILE"));
	ddString file_buffer = read_file(file_path.cstr);
	sizet token_count;
	struct token* tokens = lexer_main(file_buffer, &token_count);
	struct buffer code = generate_main(tokens, token_count);
	write_file(args_get_value(make_constant_ddString("-o")).cstr, code);
	return 0;
}

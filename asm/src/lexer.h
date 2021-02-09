#ifndef __rmasm_lexer_h__
#define __rmasm_lexer_h__

#include "./utils.h"


struct token* lexer_main(ddString file, sizet* token_count);
static void tokenize(char chr);
static char next(void);
static char peek(int offset);
static void reset_literal(void);
static void set_token(int type, union opc opc);
static int get_register(ddString str);
static bool is_number(ddString str);
static bool is_string(ddString str);
static bool is_register(ddString str);
static int is_prefix(ddString str);
static int is_command(ddString str);
static bool is_character(char chr);

static ddString literal;
static char* file_stream;
static char* file_stream_end;
static struct token* tokens;
static sizet token_count;

struct token* lexer_main(ddString file, sizet* _token_count)
{
	reset_literal();
	file_stream = file.cstr;
	file_stream_end = file.cstr+file.length;
	tokens = make(struct token, 1000);
	token_count = 0;
	while (file_stream != file_stream_end)
	{
		tokenize(next());
	}
	*_token_count = token_count;
	return tokens;
}

static void tokenize(char chr)
{
	switch (chr)
	{
		case 'a' ... 'z':
		case 'A' ... 'Z':
		case '0' ... '9':
		case '"':
		{
			ddString_push_char_back(&literal, chr);
			while (is_character(peek(0)))
			{
				chr = next();
				ddString_push_char_back(&literal, chr);
			}
			int value;
			if (is_number(literal))
			{
				union opc opc;
				opc.num = ddString_to_int(literal);
				set_token(TKN_NUMBER, opc);
			}
			else if (is_string(literal))
			{
				union opc opc;
				opc.str = literal;
				set_token(TKN_STRING, opc);
			}
			else if (is_register(literal))
			{
				union opc opc;
				opc.reg = get_register(literal);
				set_token(TKN_REGISTER, opc);
			}
			else if ((value = is_prefix(literal)) != -1)
			{
				union opc opc;
				opc.pfx = value;
				set_token(TKN_PREFIX, opc);
			}
			else if ((value = is_command(literal)) != -1)
			{
				union opc opc;
				opc.com = value;
				set_token(TKN_COMMAND, opc);
			}
			else
			{
				ddPrintf("UNIDENTIFYIED LITERL %s\n", literal.cstr);
			}
			reset_literal();
		} break;
	}
}

static char next(void)
{
	return (file_stream != file_stream_end) ? (*file_stream++) : (0);
}
static char peek(int offset)
{
	return ((file_stream+offset) < file_stream_end) ? (*(file_stream+offset)) : (0);
}
static void reset_literal(void)
{
	literal = make_ddString_length(" ", 25);
	literal.length = 0;
}
static void set_token(int type, union opc opc)
{
	tokens[token_count].type = type;
	tokens[token_count].opc = opc;
	token_count++;
}
static int get_register(ddString str)
{
	int id = str.cstr[0];
	ddString reg_num_str = str;
	reg_num_str.cstr++;
	int reg_num = ddString_to_int(reg_num_str);
	return REGISTER_IDS[id] + reg_num;
}
static bool is_number(ddString str)
{
	for (int i = 0; i < str.length; i++)
		if (!(str.cstr[i] >= '0' && str.cstr[i] <= '9'))
			return false;
	return true;
}
static bool is_string(ddString str)
{
	return (str.cstr[0] == '"' && str.cstr[str.length-1] == '"');
}
static bool is_register(ddString str)
{
	for (int i = 1; i < str.length; i++)
		if (!(str.cstr[i] >= '0' && str.cstr[i] <= '9'))
			return false;
	return !(str.cstr[0] >= '0' && str.cstr[0] <= '9');
}
static int is_prefix(ddString str)
{
	for (int i = 0; i < sizeof(PREFIX_OPC_IDS)/sizeof(struct opc_id); i++)
	{
		if (ddString_compare_cstring(str, PREFIX_OPC_IDS[i].cstr)) return PREFIX_OPC_IDS[i].id;
	}
	return -1;
}
static int is_command(ddString str)
{
	for (int i = 0; i < sizeof(OPERATOR_OPC_IDS)/sizeof(struct opc_id); i++)
	{
		if (ddString_compare_cstring(str,  OPERATOR_OPC_IDS[i].cstr)) return OPERATOR_OPC_IDS[i].id;
	}
	return -1;
}
static bool is_character(char chr)
{
	switch (chr)
	{
		case 'a' ... 'z':
		case 'A' ... 'Z':
		case '0' ... '9':
			return true;
			break;
		default:
			return false;
			break;
	}
}

#endif

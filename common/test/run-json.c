#include "../json.c"
#include <stdio.h>

/* AUTOGENERATED MOCKS START */
/* AUTOGENERATED MOCKS END */

int main(void)
{
	struct json_result *result = new_json_result(NULL);
	jsmntok_t *toks;
	const jsmntok_t *x;
	bool valid;
	int i;
	char *badstr = tal_arr(result, char, 256);
	const char *str;

	/* Fill with junk, and nul-terminate (256 -> 0) */
	for (i = 1; i < 257; i++)
		badstr[i-1] = i;

	json_object_start(result, NULL);
	json_add_string(result, "x", badstr);
	json_object_end(result);

	/* Parse back in, make sure nothing crazy. */
	str = json_result_string(result);

	toks = json_parse_input(str, strlen(str), &valid);
	assert(valid);
	assert(toks);

	assert(toks[0].type == JSMN_OBJECT);
	x = json_get_member(str, toks, "x");
	assert(x);
	assert(x->type == JSMN_STRING);
	assert((x->end - x->start) == 255);
	for (i = x->start; i < x->end; i++) {
		assert(cisprint(str[i]));
		assert(str[i] != '\\');
		assert(str[i] != '"');
		assert(str[i] == '?' || str[i] == badstr[i - x->start]);
	}
	tal_free(result);
	return 0;
}
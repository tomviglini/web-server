#include <core_lib/header/system.h>

#include <core_lib/header/string.h>

__attribute__((always_inline))
void
system_init(struct SYSTEM *System) {
	string_init(&System->Response.Header2, 4096);
	string_init(&System->Response.Header3, 4096);
	string_init(&System->Response.Body, 	32768);
}

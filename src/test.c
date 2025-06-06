#include <stdio.h>
#include "str.h"

int main() {
    str_init_arena(1024);
    str name = str_new("hello");
    printf("%s\n", str_cstr(name));
    str_concat(&name, str_new(" world!"));
    printf("%s\n", str_cstr(name));
    str_free_arena();
    return 0;
}

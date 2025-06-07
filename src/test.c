#include <stdio.h>
#include "str.h"
#include "log.h"

int main() {
    log_init("log.txt");
    log("hello world :D");

    str_init_arena(1024);
    str name = str_new("hello");
    printf("%s\n", str_cstr(name));
    str_concat(&name, str_new(" world!"));
    printf("%s\n", str_cstr(name));
    
    log("%d", str_eq(name, str_new("hi")));
    log("%d", str_eq(name, str_new("hello world!")));
    
    str_free_arena();
    log_free();

    return 0;
}

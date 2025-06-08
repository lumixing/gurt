#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <msgpack.h>

int main() {
    msgpack_sbuffer buf;
    msgpack_sbuffer_init(&buf);

    // msgpack_packer pk;
    // msgpack_packer_init(&pk, &buf, msgpack_sbuffer_write);

    // msgpack_pack_map(&pk, 2);

    // char *name_key = "name";
    // msgpack_pack_str_with_body(&pk, name_key, strlen(name_key));
    // char *name_value = "lumix!!1";
    // msgpack_pack_str_with_body(&pk, name_value, strlen(name_value));

    // char *age_key = "age";
    // msgpack_pack_str_with_body(&pk, age_key, strlen(age_key));
    // msgpack_pack_int(&pk, 18);

    // FILE *file = fopen("test.msg", "w");
    // fwrite(buf.data, 1, buf.size, file);
    // fclose(file);

    // msgpack_sbuffer_destroy(&buf);

    FILE *file = fopen("test.msg", "r");

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = malloc(size);
    fread(data, 1, size, file);

    msgpack_zone mempool;
    msgpack_zone_init(&mempool, 2048);

    msgpack_object obj;
    msgpack_unpack(data, size, NULL, &mempool, &obj);

    // msgpack_object_print(stdout, obj);
    // puts("");

    msgpack_object_map map = obj.via.map;

    const char *name = map.ptr[0].val.via.str.ptr;
    int name_len = map.ptr[0].val.via.str.size;
    printf("%.*s\n", name_len, name);

    int age = map.ptr[1].val.via.i64;
    printf("%d\n", age);

    msgpack_zone_destroy(&mempool);
    msgpack_sbuffer_destroy(&buf);

    free(data);
    fclose(file);

    return 0;
}

#include <cbuffer.h>
#include <assert.h>
#include <stdio.h>

# define UNITTEST(x) void utest_##x(void); void x(void) { printf(#x" ... \n"); utest_##x(); printf("pass.\n"); } void utest_##x(void)

UNITTEST(test_cbuffer_indexof)
{
    t_cbuffer cbuf;
    const char *str = "ajwf wefh hello vnkjr";

    // TEST 1
    cbuffer_reset(&cbuf);
    cbuf.tail = 0;
    cbuf.head = strlen(str);
    memcpy(cbuf.buffer, str, strlen(str));
    // printf("cbuffer_indexof(&cbuf, 'hello): %ld\n", cbuffer_indexof(&cbuf, "hello"));
    assert(cbuffer_indexof(&cbuf, "hello") == 10);

    // TEST 2
    cbuffer_reset(&cbuf);
    cbuf.tail = 14;
    cbuf.head = 14 + strlen(str);
    memset(cbuf.buffer, 'X', CBUFFSIZE);
    memcpy(cbuf.buffer + cbuf.tail, str, strlen(str));
    assert(cbuffer_indexof(&cbuf, "hello") == 24);

    // TEST 3 => empty string
    assert(cbuffer_indexof(&cbuf, "") == -1);

    // TEST 4 => [Y Y Z 0 0 ... 0 X X X X ...]
    //                |           |
    //                h           t
    cbuffer_reset(&cbuf);

    cbuf.tail = 14;
    memset(cbuf.buffer + cbuf.tail, 'X', CBUFFSIZE - cbuf.tail);

    cbuf.head = 3;
    memset(cbuf.buffer, 'Z', 3);

    cbuf.buffer[0] = 'Y';
    cbuf.buffer[1] = 'Y';

    assert(cbuffer_indexof(&cbuf, "YY") == 0);
    assert(cbuffer_indexof(&cbuf, "YZ") == 1);
    assert(cbuffer_indexof(&cbuf, "YYY") == -1);

    // TEST 5 => [D E F 0 0 ... 0 X X X X A B C]
    //                |           |
    //                h           t
    cbuffer_reset(&cbuf);

    cbuf.tail = 28;
    memset(cbuf.buffer + cbuf.tail, 'X', CBUFFSIZE - cbuf.tail);
    memcpy(cbuf.buffer + CBUFFSIZE - strlen("ABC"), "ABC", strlen("ABC"));

    cbuf.head = 3;
    memcpy(cbuf.buffer, "DEF", strlen("DEF"));

    // printf("cbuffer_indexof(&cbuf, 'ABCDEF'): %ld\n", cbuffer_indexof(&cbuf, "ABCDEF"));
    assert(cbuffer_indexof(&cbuf, "ABCDEF") == CBUFFSIZE - strlen("ABC"));
    assert(cbuffer_indexof(&cbuf, "AB") == CBUFFSIZE - strlen("ABC"));
    assert(cbuffer_indexof(&cbuf, "EF") == 1);

    // TEST 6 => [... X X X ... X X ... H E L L O ...]
    //                |           |
    //                t           h
    cbuffer_reset(&cbuf);
    memset(cbuf.buffer, 'X', CBUFFSIZE);
    cbuf.tail = 28;
    cbuf.head = 78;
    memcpy(cbuf.buffer + 150, "HELLO", 5);

    assert(cbuffer_indexof(&cbuf, "HELLO") == -1);

    // TEST 7 => [... H E L L 0 ... X X ...]
    //                    |           |
    //                    t           h
    cbuffer_reset(&cbuf);
    memset(cbuf.buffer, 'X', CBUFFSIZE);
    cbuf.tail = 28;
    cbuf.head = 78;
    memcpy(cbuf.buffer + 26, "HELLO", 5);

    assert(cbuffer_indexof(&cbuf, "HELLO") == -1);
}

int main()
{
    test_cbuffer_indexof();
}
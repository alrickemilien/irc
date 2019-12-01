#include <cbuffer/cbuffer.h>
#include <assert.h>
#include <stdio.h>

#define UNITTEST(x)           \
    void utest_##x(void);     \
    void x(void)              \
    {                         \
        printf(#x " ... \n"); \
        utest_##x();          \
        printf("pass.\n");    \
    }                         \
    void utest_##x(void)

UNITTEST(test_cbuffer_indexof)
{
    t_cbuffer   cbuf;
    const char *str = "ajwf wefh hello vnkjr";

    // TEST 1
    cbuffer_reset(&cbuf);
    cbuf.tail = 0;
    cbuf.head = strlen(str);
    memcpy(cbuf.buffer, str, strlen(str));
    // printf("cbuffer_indexof(&cbuf, 'hello): %ld\n", cbuffer_indexof(&cbuf,
    // "hello"));
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

    // printf("cbuffer_indexof(&cbuf, 'ABCDEF'): %ld\n", cbuffer_indexof(&cbuf,
    // "ABCDEF"));
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

    // TEST 8
    cbuffer_reset(&cbuf);
    const char *cmd =
        "PASS dummy_password\x0D\x0ANICK client_1\x0D\x0AUSER client1 "
        "microsoft.com :Client One\x0D\x0A";
    memcpy(cbuf.buffer, cmd, strlen(cmd));
    cbuf.tail = 21;
    cbuf.head = 76;

    assert(cbuffer_indexof(&cbuf, "\x0D\x0A") == 34);

    // TEST 9
    cbuffer_reset(&cbuf);
    memset(cbuf.buffer, 'X', CBUFFSIZE);
    cbuf.buffer[CBUFFSIZE - 2] = 't';
    cbuf.buffer[CBUFFSIZE - 1] = 'o';
    cbuf.buffer[0] = 't';
    cbuf.buffer[1] = 'o';
    cbuf.tail = 3;
    cbuf.head = 2;
    assert(cbuffer_indexof(&cbuf, "toto") == CBUFFSIZE - 2);

    // TEST 10
    cbuffer_reset(&cbuf);
    const char A[] =
        ":q\x0D\x0APRIVMSG &hub :q\x0D\x0APRIVMSG &hub :q\x0D\x0APRIVMSG &hub "
        ":q\x0D\x0APRIVMSG &hub :q\x0D\x0A\x0APRIVMSG &hub :qq\x0D\x0APRIVMSG "
        "&hub :q\x0D\x0APRIVMSG &hub :q\x0D\x0APRIVMSG &hub :q\x0D\x0APRIVMSG "
        "&hub :q\x0D\x0APRIVMSG &hub :q\x0D\x0APRIVMSG &hub :q\x0D\x0APRIVMSG "
        "&hub :q\x0D\x0APRIVMSG &hub :q\x0D\x0APRIVMSG &hub :q\x0D\x0APRIVMSG "
        "&hub :q\x0D\x0APRIVMSG &hub :q\x0D\x0APRIVMSG &hub :q\x0D\x0APRIVMSG "
        "&hub :q\x0D\x0APRIVMSG &hub :q\x0D\x0APRIVMSG &hub :q\x0D\x0APRIVMSG "
        "&hub :q\x0D\x0APRIVMSG &hub :q\x0D\x0APRIVMSG &hub :q\x0D\x0APRIVMSG "
        "&hub :q\x0D\x0APRIVMSG &hub :q\x0D\x0APRIVMSG &hub :q\x0D\x0APRIVMSG "
        "&hub :q\x0D\x0APRIVMSG &hub :q\x0D\x0APRIVMSG &hub :q\x0D\x0APRIVMSG "
        "&hub";
    memcpy(cbuf.buffer, A, sizeof(A) - 1);
    cbuf.tail = 73;
    cbuf.head = 73;
    assert(cbuffer_indexof(&cbuf, "\x0D\x0A") == -1);
}

UNITTEST(test_cbuffer_putstr)
{
    t_cbuffer cbuf;

    // TEST 1
    cbuffer_reset(&cbuf);
    cbuffer_putstr(&cbuf, "ajwf wefh hello vnkjr");
    assert(cbuffer_indexof(&cbuf, "ajwf wefh hello vnkjr") == 0);

    // TEST 2
    cbuffer_putstr(&cbuf, "yaya");
    // printf("cbuffer_indexof(&cbuf, 'yaya'): %ld\n", cbuffer_indexof(&cbuf,
    // "yaya"));
    assert(cbuffer_indexof(&cbuf, "yaya") == 21);

    // TEST 3
    cbuf.head = CBUFFSIZE - 2;
    cbuffer_putstr(&cbuf, "toto");
    printf("cbuffer_indexof(&cbuf, 'toto'): %ld\n",
           cbuffer_indexof(&cbuf, "toto"));
    assert(cbuffer_indexof(&cbuf, "toto") == -1);

    // TEST 4
    memset(cbuf.buffer, 'X', CBUFFSIZE);
    cbuf.buffer[CBUFFSIZE] = 0;
    cbuf.tail = 3;
    cbuf.head = CBUFFSIZE - 2;
    cbuffer_putstr(&cbuf, "toto");
    // printf("::%s\n", cbuf.buffer);
    printf("cbuffer_indexof(&cbuf, 'toto'): %ld\n",
           cbuffer_indexof(&cbuf, "toto"));
    assert(cbuffer_indexof(&cbuf, "toto") == CBUFFSIZE - 2);
}

UNITTEST(test_cbuffer_put)
{
    t_cbuffer cbuf;
    size_t    index;

    const char msg[] =
        "PRIVMSG &hub "
        ":AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\x0D\x0A";

    // TEST 1
    cbuffer_reset(&cbuf);
    cbuffer_put(&cbuf, "USER aaaaaaa aaaaaaa 127.0.0.1 aaaaaaa\x0D\x0A",
                strlen("USER aaaaaaa aaaaaaa 127.0.0.1 aaaaaaa\x0D\x0A"));
    cbuffer_put(&cbuf, msg, sizeof(msg) - 1);
    cbuffer_put(&cbuf, msg, sizeof(msg) - 1);

    cbuf.tail = 366;
    cbuf.head = 112;

    // cbuffer_debug(&cbuf);

    index = cbuffer_indexof(&cbuf, "\x0D\x0A");
    // printf("cbuffer_indexof(&cbuf, '\\x0D\\x0A'): %ld\n", index);

    assert(index == 38);
}

int main()
{
    test_cbuffer_indexof();
    test_cbuffer_putstr();
    test_cbuffer_put();
}

#include "irc.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    t_env e;
    
    server_create(&e, 5555);

    return (0);
}
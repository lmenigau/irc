#include "irc.hpp"

#define COMMAND_COUNT 2

void    _pass(std::list<std::string> * args)
{
    (void)args;
    std::cout << "PASS COMMAND\n";
}

void    _user(std::list<std::string> * args)
{
    (void)args;
    std::cout << "USER COMMAND\n";
}
void    handler( std::list<std::string> * args)
{
    std::string commands[COMMAND_COUNT] = { "PASS", "USER" };
    void (*handlers[COMMAND_COUNT])( std::list<std::string> * ) = { &_pass, &_user };
    for (size_t i = 0; i < COMMAND_COUNT; i++)
    {
        if (!args->front().compare(commands[i]))
        {
            handlers[i](args);
            return ;
        }
    }
    std::cout << "COMMAND DO NOT EXIST (YET?)\n";
}

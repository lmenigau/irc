#include "irc.hpp"
#include "client.hpp"

#define COMMAND_COUNT 4

void _pass(std::list<std::string>* args, client& c) {
    (void)args;
    std::cout << "PASS COMMAND\n";
}

void _user(std::list<std::string>* args, client& c) {
    (void)args;
    std::cout << "USER COMMAND\n";
}

void _join(std::list<std::string>* args, client& c) {
    std::cout << c.nick << "wants to join \n";
}
void _nick(std::list<std::string>* args, client& c) {
    (void)args;
    if (args->empty()) {
        c.reply("431\r\n");
    } else {
        c.nick = args->front();
        c.isregistered = true;
        c.reply("001\r\n");
        std::cout << "nickname is " << c.nick << "\n";
    }
}

void handler(std::list<std::string>* args, client& c) {
    std::string commands[COMMAND_COUNT] = {"PASS", "USER", "NICK", "JOIN"};
    void (*handlers[COMMAND_COUNT])(std::list<std::string>*, client & c) = {
        &_pass, &_user, &_nick, &_join};
    for (size_t i = 0; i < COMMAND_COUNT; i++) {
        if (!args->front().compare(commands[i])) {
            args->pop_front();
            handlers[i](args, c);
            return;
        }
    }
    std::cout << "COMMAND DO NOT EXIST (YET?)\n";
}

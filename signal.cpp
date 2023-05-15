#include <csignal>
#include "ircserv.hpp"

void interupt_handler(int signal) {
     if (signal != SIGINT)
        return;
    ircserv::stop();
    exit(0);
}
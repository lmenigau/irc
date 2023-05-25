#include <csignal>
#include "ircserv.hpp"
#include "utils.hpp"

std::sig_atomic_t ircserv::is_signaled = false;
void              interupt_handler( int signal ) {
    if ( signal != SIGINT )
        return;
    ircserv::is_signaled = true;
    logger( "WARNING", "INTERRUPT DETECTED" );
    ircserv::stop();
}

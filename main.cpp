#include "irc.hpp"
std::ostream& operator<<(std::ostream& os, epoll_event& ev) {
    if (ev.events & EPOLLHUP)
        os << "HUP";
    if (ev.events & EPOLLIN)
        os << "IN";
    if (ev.events & EPOLLOUT)
        os << "OUT";
    if (ev.events & EPOLLRDHUP)
        os << "RDHUP";
    if (ev.events & EPOLLPRI)
        os << "PRI";
    if (ev.events & EPOLLERR)
        os << "ERR";
    os << ", " << ev.data.ptr << "\n";
    return os;
}
#include "ircserv.hpp"

int pollfd;
int tcp6_socket;
client clients[1024];

void accept_client(epoll_event& ev) {
    sockaddr_in6 addr = {};
    //socklen_t addrlen = sizeof(sockaddr_in6);
    (void)ev;
    int fd = accept(tcp6_socket, NULL, 0);
    std::cout << fd << " " << addr.sin6_port << '\n';
    if (fd >= 0) {
        clients[fd].fd = fd;
        clients[fd].start = 0;
        epoll_event event = {EPOLLIN, {.ptr = &clients[fd]}};
        epoll_ctl(pollfd, EPOLL_CTL_ADD, fd, &event);
        clients[fd].buf.reserve(512);
    } else
        std::cerr << "accept error\n";
}

void process_events(epoll_event& ev) {
    // std::cout << ev;
    if (ev.events & EPOLLIN) {
        if (ev.data.fd == tcp6_socket) {
            accept_client(ev);
        } else {
            client* c = (client*)ev.data.ptr;
            char buf[512];
            size_t len = read(c->fd, buf, 512);
            if (len == 0) {
                close(c->fd);
                std::cerr << "closed : " << c->fd << '\n';
                return;
            }
            c->buf.append(buf, len);
            for (;;) {
                size_t pos = c->buf.find("\n");
                if (pos == std::string::npos)
                    break;
                std::cerr << c->fd << ':' << c->buf.substr(0, pos) << '\n';
                c->buf.erase(0, pos + 1);
            }
            send(c->fd, "message de test\n", strlen("message de test\n"), 0);
        }
    } else if (ev.events & EPOLLOUT) {
    } else {
        std::cerr << ev;
    }
}

int main(int ac, char** av) {
    if ( ac != 3)
    {
        std::cout << "usage : ./ircserv <port> <password>" << std::endl;
        return (1);
    }
    ircserv::initialisation(av[2], av[1]);
    if (ircserv::failed())
    {
        std::cout << "FATAL : initialisation has failed !\n";
        return (1);
    }
    ircserv::start(); 
}

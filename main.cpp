
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

int pollfd;
int tcp6_socket;
client clients[1024];

void accept_client(epoll_event& ev) {
    sockaddr_in6 addr = {};
    socklen_t addrlen = sizeof(sockaddr_in6);
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
                std::cerr << "close" << c->fd << '\n';
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
        }
    } else if (ev.events & EPOLLOUT) {
    } else {
        std::cerr << ev;
    }
}

int main(int ac, char **av) {
  tcp6_socket = socket(AF_INET6, SOCK_STREAM, 0);
  int a = 1;
  std::string *test = parse("je suis un magnifique papillon !");
  setsockopt(tcp6_socket, SOL_SOCKET, SO_REUSEADDR, &a, sizeof(a));
  struct sockaddr_in6 addr = {AF_INET6};
  addr.sin6_port = htons(6667);
  addr.sin6_addr = in6addr_any;
  int ret = bind(tcp6_socket, (sockaddr *)&addr, sizeof(addr));
  if (ret < 0)
    std::perror("ircserv");
  listen(tcp6_socket, 256);
  sockaddr_in6 peer_addr = {};
  socklen_t len = sizeof(peer_addr);
  pollfd = epoll_create(1);
  epoll_event event = {EPOLLIN, {.fd = tcp6_socket}};
  epoll_ctl(pollfd, EPOLL_CTL_ADD, tcp6_socket, &event);
  for (;;) {
    epoll_event events[64];
    int nev = epoll_wait(pollfd, events, 64, -1);
    std::cerr << "nev:" << nev << "\n";
    for (int i = 0; i < nev; i++) {
      process_events(events[i]);
    }
}

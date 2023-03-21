#include <asm-generic/socket.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <ostream>
#include <sstream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
std::ostream &operator<<(std::ostream &os, epoll_event &ev) {
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
  os << ", " << ev.data.fd << "\n";
  return os;
}

struct client {
  int fd;
  int start;
  int end;
  char buf[512];
  std::stringstream str;
};

client clients[1024];

int main(int ac, char **av) {
  int tcp6_socket = socket(AF_INET6, SOCK_STREAM, 0);
  int a = 1;
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
  int pollfd = epoll_create(1);
  epoll_event event = {EPOLLIN, {.fd = tcp6_socket}};
  epoll_ctl(pollfd, EPOLL_CTL_ADD, tcp6_socket, &event);
  for (;;) {
    epoll_event events[64];
    int nev = epoll_wait(pollfd, events, 64, -1);
    std::cout << "nev:" << nev << "\n";
    for (int i = 0; i < nev; i++) {
      if (events[i].data.fd == tcp6_socket) {
        int fd = accept(tcp6_socket, (sockaddr *)&peer_addr, &len);
        clients[fd].fd = fd;
        clients[fd].start = 0;
        epoll_event event = {EPOLLIN, {.ptr = &clients[fd]}};
        epoll_ctl(pollfd, EPOLL_CTL_ADD, fd, &event);
      } else {
        client *c = (client *)events[i].data.ptr;
        size_t len = read(c->fd, c->buf + c->start, 512 - c->start);
        if (len == 0)
          close(c->fd);
        for (void *lf; (lf = std::memchr(c->buf, '\n', len)); c->start += len) {
          std::cout << "len:" << len << "\n";
          std::cout.write(c->buf, (char *)lf - c->buf);
		  std::cout << std::endl;
		  len = read(c->fd, c->buf + c->start, 512 - c->start);
		  if (len == 0)
		  {
			close(c->fd);
			break	;
		  }
        }
      }
    }
  }
}

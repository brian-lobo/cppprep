#pragma once

#include <iostream>
#include <vector>
#include <cstring>
#include <chrono>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>



namespace cppprep::sockets
{

  class Server
  {
    public:

      bool setReceiveBufferSize() {
        int rcvbuf = 16 * 10124 * 1024;
        setsockopt(_serverFd, SOL_SOCKET, SO_RCVBUF, &rcvbuf, sizeof(rcvbuf));

        return true;
      }

      bool bind() const {
        sockaddr_in addr{AF_INET, SOCK_STREAM, IPPROTO_TCP};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons(PORT);

        if (::bind(_serverFd, (sockaddr*)&addr, sizeof(addr)) < 0)
        {
          perror("bind");
          return false;
        }

        return true;
      }

      bool createEpollSocket() {
        _epoll_fd = epoll_create(EPOLL_CLOEXEC);
        if (_epoll_fd < 0){
          perror("epoll_create");
          return false;
        }
        return true;
      }

      bool acceptIncomingClientConnection() {

        while (true)
        {
          sockaddr_in client_addr{};
          int len = sizeof(client_addr);
          int client_fd = accept(_serverFd, (sockaddr*)&client_addr, &len);
          if (client_fd < 0) break;

          set_nonblocking(client_fd);
          std::cout << "New client connected: "
                    << inet_ntoa(client_addr.sin_addr) << std::endl;
          ev.events = EPOLLIN | EPOLLET;
          ev.data.fd = client_fd;
          epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);
        }
        return true;
      }

      std::string readIncomingClientData(int fd) {
        char buffer[BUFFER_SIZE];
        std::string data(BUFFER_SIZE,' ');
        size_t total_received{0};
        bool readData{false};

        while (true)
        {
          ssize_t bytes = recv(fd,buffer, BUFFER_SIZE, 0);
          if (bytes > 0 )
          {
            total_received += bytes;
            std::cout << "\rReceived " << bytes << " bytes | Total: "
                      << total_received/(1024*1024) << " MB" << std::flush;
            data.append(buffer, bytes);
            readData = true;
          }
          else if (bytes == 0)
          {
            std::cout << "\nClient disconnected." << std::endl;
            close(fd);
            break;
          }
          else
          {
            if (errno != EAGAIN&& errno != EWOULDBLOCK)
            {
              perror("recv");
              close(fd);
            }
            break;
          }
        }

        if (readData)
          std::cout << "Data : " <<  data << std::endl;

        return data;
      }

      bool start() {
        _serverFd = socket(AF_INET, SOCK_STREAM, 0);
        if (_serverFd == -1)
        {
          perror("socket");
          return false;
        }

        int opt{-1};
        setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        if (!setReceiveBufferSize()) return false;

        if (!bind()) return false;

        ::listen(_serverFd, MAX_EVENTS);
        set_nonblocking(_serverFd);


        if (!createEpollSocket()) return false;

        epoll_event ev{};
        ev.events = EPOLLIN | EPOLLET;
        ev.data.fd = _serverFd;
        epoll_ctl(_serverFd, EPOLL_CTL_ADD, _serverFd, &ev);

        std::vector<epoll_event> events(MAX_EVENTS);
        auto start = std::chrono::high_resolution_clock::now();

        std::cout << "epoll receiver listening on port " << PORT << " (Edge-Triggered)\n";

        while (true) {

          int nfds = epoll_wait(_epoll_fd, events.data(), MAX_EVENTS, -1);
          if (nfds < 0) {
            perror("epoll_wait");
            break;
          }

          for (int i=0; i < nfds; ++i)
          {
            int fd = events[i].data.fd == _serverFd;

            if (fd == _serverFd)
            {
              acceptIncomingClientConnection();
            }
            else
            {
              readIncomingClientData(fd);
            }
          }
        }

      }

    private:
      int set_nonblocking(int fd) {
        int flags = fcntl(fd, F_GETFL, 0);
        return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
      }

      int _serverFd{0};
      int _epoll_fd{0};
      static constexpr int PORT{65432};
      static constexpr int MAX_EVENTS{16};
      static constexpr int BUFFER_SIZE{64 * 1024};   // 64 KB
  };
}
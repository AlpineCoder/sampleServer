#ifndef SIMPLESOCKET_H
#define SIMPLESOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

enum ProtocolVersion {IPV4, IPV6, ANY};

// Creates a simple TCP stream socket
// adding a feature
class SimpleSocket
{
public:
  SimpleSocket(int port_number, ProtocolVersion pv);
  ~SimpleSocket();
  void CreateSocket();
  void Listen();

private:

  static void sigchld_handler(int s);
  void * get_in_addr(struct sockaddr *sa);

  int port_number;
  ProtocolVersion pv;

  int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_storage their_addr; // connector's address information
  socklen_t sin_size;
  struct sigaction sa;
  int yes=1;
  char s[INET6_ADDRSTRLEN];
  int rv;


};

#endif // SIMPLESOCKET_H

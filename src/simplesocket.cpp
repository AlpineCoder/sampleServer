#include <stdexcept>
#include <string>
#include "../include/simplesocket.h"

// we use BSD sockets here for fun!

SimpleSocket::SimpleSocket(int port_number, ProtocolVersion pv)
{
  this->port_number = port_number;
  this->pv = pv;
}

SimpleSocket::~SimpleSocket(){}

void SimpleSocket::CreateSocket(){
  memset(&hints, 0, sizeof hints);

  switch (pv){
    case IPV4:
      hints.ai_family = AF_INET;
      break;
    case IPV6:
      hints.ai_family = AF_INET6;
      break;
    default:
      hints.ai_family = AF_UNSPEC;
    }


  //hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // use my IP

  if ((rv = getaddrinfo(NULL, std::to_string(port_number).c_str(), &hints, &servinfo)) != 0) {
      throw new std::runtime_error(gai_strerror(rv));
    }

  // loop through all the results and bind to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next) {
      if ((sockfd = socket(p->ai_family, p->ai_socktype,
                           p->ai_protocol)) == -1) {
          perror("server: socket");
          continue;
        }

      if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                     sizeof(int)) == -1) {
          perror("setsockopt");
          throw new std::runtime_error("setsockopt failed");
        }

      if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
          close(sockfd);
          perror("server: bind");
          continue;
        }
      break;
    }

  freeaddrinfo(servinfo); // all done with this structure

  if (p == NULL)  {
      fprintf(stderr, "server: failed to bind\n");
      exit(1);
    }

  //TODO: Parameterize second argument

  if (listen(sockfd, 10) == -1) {
      perror("listen");
      exit(1);
    }

  sa.sa_handler = sigchld_handler; // reap all dead processes
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
      perror("sigaction");
      exit(1);
    }
}

void SimpleSocket::Listen(){
  while(1) {  // main accept() loop
      sin_size = sizeof their_addr;
      new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
      if (new_fd == -1) {
          perror("accept");
          continue;
        }

      inet_ntop(their_addr.ss_family,
                get_in_addr((struct sockaddr *)&their_addr),
                s, sizeof s);
      printf("server: got connection from %s\n", s);

      if (!fork()) { // this is the child process
          close(sockfd); // child doesn't need the listener
          if (send(new_fd, "Hello, world!\n", 14, 0) == -1){
              perror("send");
            }
          close(new_fd);
          exit(0);
        }
      close(new_fd);  // parent doesn't need this
    }
}

void SimpleSocket::sigchld_handler(int s){
  // waitpid() might overwrite errno, so we save and restore it:
  int saved_errno = errno;

  while(waitpid(-1, NULL, WNOHANG) > 0);

  errno = saved_errno;
}

void * SimpleSocket::get_in_addr(sockaddr *sa){
  if (sa->sa_family == AF_INET) {
      return &(((struct sockaddr_in*)sa)->sin_addr);
    }

  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

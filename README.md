# sampleServer
Very simple stream server based on [Beej's Guide to Network Programming](http://beej.us/guide/bgnet/html/multi/index.html) stream server code.

## Getting started

### Prerequisites

You need cmake 3.1 or higher and a compiler that supports C++14. Tested with g++ 5.4, 5.5 and 7.2.1.

### Building

```
$ git clone https://github.com/AlpineCoder/sampleServer
$ cd sampleServer
$ cmake .
$ make
```

### Running it

```
$ ./sampleServer <-p port_number> [-v 4|6]
```

You can connect to it using telnet. For example if the server is listening on port 1234:
```
$ telnet ::1 1234
Trying ::1...
Connected to ::1.
Escape character is '^]'.
Hello, world!
Connection to ::1 closed by foreign host.
```

To stop the server, just use ctrl-c.


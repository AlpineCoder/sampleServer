#include <iostream>
#include <sstream>
#include <climits>
#include <memory>
#include <vector>

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "../include/simplesocket.h"

// checks if the portnumber is ok.
// Args:
// port_number -> reference that will be updated if the port number is correct
// port_param -> value from argument
bool CheckPortNumber(int & port_number, const char* port_param);

// checks if ip version is ok.
// Args:
// protocol_version -> expected to be 4 or 6
// port_param -> value from argument
bool CheckIPVersion(int & protocol_version, char* protocol_param);

int main(int argc, char** argv)
{
  // Just for the output for now
  std::vector<std::string> ip_protocol_versions;
  ip_protocol_versions.push_back("IPv4");
  ip_protocol_versions.push_back("IPv6");
  ip_protocol_versions.push_back("Any");


  int port_number = 0, protocol_version = 0;

  // Fun with good old getopt!
  int opt,p = -1,v = -1;

  ProtocolVersion pv = ANY;

  while((opt = getopt(argc, argv,"p:v:")) != EOF)
    switch(opt){
      case 'p' :
        p = 1;
        if(CheckPortNumber(port_number, optarg)){}
        else{
            std::cerr << "Port number not valid\n";
            return 1;
          }
        break;

      case 'v' :
        v = 1;
        //std::cout << "value of -v is: " << optarg << "\n";
        if(CheckIPVersion(protocol_version, optarg)){
          }
        else{
            std::cerr << "Protocol version not valid\n";
            return 1;
          }
        break;

      default:
        return 1;
      }

  if( p == -1){
      std::cerr << "-p is mandatory!\n";
      return 1;
    }

  switch (protocol_version){
    case 4 :
      pv = IPV4;
      break;
    case 6 :
      pv = IPV6;
      break;
    default:
      pv = ANY;
      break;
    }

  std::cout << "Starting simpleServer on " << port_number << " with Protocol " << ip_protocol_versions[pv] << "\n";

  // Start working with the socket
  auto socket = std::make_unique<SimpleSocket>(port_number, pv);
  try{
    socket->CreateSocket();
    socket->Listen();
  }
  catch(std::runtime_error e){
    std::cerr << e.what() << "\n";
  }

  return 0;
}

bool CheckPortNumber(int & port_number, const char* port_param){
  std::stringstream ss(port_param);
  return (ss >> port_number && port_number < USHRT_MAX) ? true : false;
}

bool CheckIPVersion(int &protocol_version, char *protocol_param){
  std::stringstream ss(protocol_param);
  return (ss >> protocol_version && (protocol_version == 4 || protocol_version == 6)) ? true : false;
}

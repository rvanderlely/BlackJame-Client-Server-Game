#include <sys/socket.h> //socket functions
#include <arpa/inet.h> //socket address structure
#include <errno.h> //for EINTR
#include <string.h> //bezero()
#include <stdio.h> //printf(), read(), etc
#include <stdlib.h> //exit()
#include <time.h> //Used for srand((unsigned) time(NULL)) command
#include <vector> //For the dealer/player hand
//#include "blackjack.cpp"
#define SERV_PORT 4455 //server port 
#define SA struct sockaddr //sockaddress structure
#define LISTENQ 1024 //bind()'s backlog
#define MAXLINE 4096
#define BUFFERSIZE 8192

#include <algorithm> // Needed for transform in the hit or stay function
#include <cstring>
#include <iostream>


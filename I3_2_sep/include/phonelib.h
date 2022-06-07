#pragma once
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

int make_socket_server(int port, int max_accept);
int make_socket(char* ip, int port);
int make_connect_server(int ss);
void die(char* s);
void data_throwing(struct timeval t0);


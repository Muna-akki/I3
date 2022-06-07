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
#include "./../include/phonelib.h"
#include "./../include/freqlib.h"


int make_socket_server(int port, int max_accept){
    int s = socket(PF_INET, SOCK_STREAM, 0);
    if(s==-1){
        die("socket");
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    int n = bind(s, (struct sockaddr*)&addr, sizeof(addr));
    if(n==-1){
        die("bind");
    }
    listen(s,max_accept);
    return s;
}

int make_connect_server(int ss){
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    int s = accept(ss, (struct sockaddr*) &client_addr, &len);
    if(s==-1){
        die("accept");
    }
    return s;
}

int make_socket(char* ip, int port){
    int s = socket(PF_INET, SOCK_STREAM, 0);
    if(s==-1){
        die("socket");
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    int n = inet_aton(ip,&addr.sin_addr);
    if(n==0){
        die("inet_aton");
    }
    addr.sin_port = htons(port);
    int net = connect(s,(struct sockaddr *)&addr, sizeof(addr));
    if(net==-1){
        die("connect");
    }
    return s;
}

void die(char* s){
    perror(s);
    exit(1);
}

void data_throwing(struct timeval t0){
    struct timeval t1;
    gettimeofday(&t1,NULL);
    double distance_time = ((double)t1.tv_sec*1000000+t1.tv_usec)-((double)t0.tv_sec*1000000+t0.tv_usec); //micro_second
    double throw_data = distance_time*44100*2/1000000; //44100 sampling/second * 52byte(16bit)/sampling
    int N_ex = 1000;
    unsigned char data_send_ex[N_ex];
    int n_send_ex;
    while(throw_data>0){
        n_send_ex = read(0,data_send_ex,N_ex);
        if(n_send_ex==-1){
            die("read");
        }
        if(n_send_ex==0){
            break;
        }
        throw_data = throw_data-n_send_ex;
    }
}



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


int main(int argc, char** argv){
    int count_connect = 0;
    int max_accept = 10;
    int s = -1;
    int ss = -1;
    struct timeval t0;
    gettimeofday(&t0, NULL);
    
    int is_server = -1;
    if(argc==2){
        is_server = 1;
        int port = (int)atol(argv[1]);
        ss = make_socket_server(port, max_accept);
        s = make_connect_server(ss);
        count_connect++;
    }else if(argc==3){
        is_server = 0;
        int port = (int)atol(argv[2]);
        s = make_socket(argv[1],port);
        count_connect++;
    }else{
        die("argument");
    }
    if(is_server!=0 && is_server!=1){
        die("server or client");
    }
    int N = 2;
    unsigned char data_recv[N];
    unsigned char data_send[N];
    int n_recv;
    int n_send;

    data_throwing(t0);

    while(1){
        n_send = read(0,data_send,N);
        if(n_send==-1){
            die("read");
        }
        if(n_send==0){
            break;
        }
        send(s,data_send,n_send*sizeof(unsigned char),0);
        
        n_recv = recv(s,data_recv,N,0);
        if(n_recv==-1){
            die("receive");
        }
        if(n_recv==0){
            break;
        }
        write(1,data_recv,n_recv*sizeof(unsigned char));
    }
    close(s);
}

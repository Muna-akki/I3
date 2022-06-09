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

//client = IP, port, fmin,fmax,slide
//server = port, fmin,fmax, slide
int main(int argc, char** argv){
    struct timeval t0;
    gettimeofday(&t0, NULL);
    long fmin = -1;
    long fmax = -1;
    int slide = -1;
    int count_connect = 0;
    int max_accept = 10;
    int s = -1;
    int ss = -1;
    int is_server = -1;
    if(argc==5){
        //server
        fmin = atol(argv[2]);
        fmax = atol(argv[3]);
        slide = (int)atol(argv[4]);
        is_server = 1;
        int port = (int)atol(argv[1]);
        ss = make_socket_server(port, max_accept);
        s = make_connect_server(ss);
        count_connect++;
    }else if(argc==6){
        //client
        fmin = atol(argv[3]);
        fmax = atol(argv[4]);
        slide = (int)atol(argv[5]);
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
    int N = 8192;
    short data_recv[N*2];
    short data_send[N*2];
    int n_recv;
    int n_send;
    data_throwing(t0);

    while(1){
        n_send = read(0,data_send,N*2);
        if(n_send==-1){
            die("read");
        }
        if(n_send==0){
            break;
        }
        touch_sound(s,N,data_send,fmin,fmax,slide);
        send(s,data_send,n_send,0);

        n_recv = recv(s,data_recv,N*2,0);
        if(n_recv==-1){
            die("receive");
        }
        if(n_recv==0){
            break;
        }
        write(1,data_recv,n_recv);
    }
    close(s);
}

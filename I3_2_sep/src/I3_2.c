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
    struct timeval t0;
    gettimeofday(&t0, NULL);
    int slide = (int)atol(argv[3]);
    int count_connect = 0;
    int max_accept = 10;
    int s = -1;
    int ss = -1;
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
    int N = 8192;
    short data_recv[N];
    short data_send[N*2];
    int n_recv;
    int n_send;

    data_throwing(t0);
    int offset = 0;
    while(1){
        n_send = read(0,data_send+offset,N);
        if(n_send==-1){
            die("read");
        }
        if(n_send==0){
            break;
        }
        
        if(n_send>=N){
            touch_sound(s,N,data_send,slide);
            offset += n_send-N;
            for(int i=0 ; i+N<=2*N-1 ; i++){
                data_send[i] = data_send[i+N];
            }
        }else{
            offset += n_send;
        }
        if(offset>N){
            touch_sound(s,N,data_send,slide);
            offset -= N;
        }
        //send(s,data_send,n_send,0);
        
        n_recv = recv(s,data_recv,N,0);
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

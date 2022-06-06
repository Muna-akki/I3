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
int make_socket_server(int port);
int make_socket(char* ip, int port);
void die(char* s);


int main(int argc, char** argv){
    int s = -1;
    struct timeval t0;
    gettimeofday(&t0, NULL);
    int is_server = -1;
    if(argc==2){
        is_server = 1;
        int port = (int)atol(argv[1]);
        int ss = make_socket_server(port);
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(struct sockaddr_in);
        s = accept(ss, (struct sockaddr*) &client_addr, &len);
        if(s==-1){
            die("accept");
        }
    }else if(argc==3){
        is_server = 0;
        int port = (int)atol(argv[2]);
        s = make_socket(argv[1],port);

    }else{
        die("argument");
    }
    
    int N = 1000;
    unsigned char data_recv[N];
    unsigned char data_send[N];
    int n_recv;
    int n_send;
    
    struct timeval t1;
    gettimeofday(&t1,NULL);
    double distance_time = ((double)t1.tv_sec*1000000+t1.tv_usec)-((double)t0.tv_sec*1000000+t0.tv_usec); //micro_second
    double throw_data = distance_time*44100*2/1000000; //44100 sampling/second * 52byte(16bit)/sampling
    
    if(is_server!=0 && is_server!=1){
        die("server? or client?");
    }

    while(throw_data>0){
        n_send = read(0,data_send,N);
        if(n_send==-1){
            die("read");
        }
        if(n_send==0){
            break;
        }
        throw_data = throw_data-n_send;
    }

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

int make_socket_server(int port){
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
    listen(s,10);
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




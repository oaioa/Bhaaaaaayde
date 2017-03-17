/*
   Simple udp server
http://www.binarytides.com/programming-udp-sockets-c-linux/
*/

#include "useful.h"

int main(void){
    struct sockaddr_in si_me;
    struct sockaddr_in si_other;
    int s; //SOCKET
    int i; //ITERATOR
    int slen = sizeof(si_other);
    int recv_len=0;

    char buf[BUFLEN];
    char message[BUFLEN];
    delete(buf,BUFLEN);
    
    int PORT2 = PORT;
    int pid = getpid();
    int pid_fils = -1;

    s = make_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    set_sockaddr_in(&si_me,PORT,INADDR_ANY);

    set_bind(s,&si_me,sizeof(si_me));
    
    int compare = 1 ; // il ya une difference

    //keep listening for data
    while(1){
        printf("I am %d Waiting for data...\n",getpid());

        delete(buf,recv_len);
        receive_message(s, buf,(struct sockaddr *) &si_other, &slen);

        //print details of the client/peer and the data received
        printf("I am %d Received packet from %s:%d\n", getpid(),inet_ntoa(si_other.sin_addr),ntohs(si_other.sin_port));
        printf("Data: %s\n" , buf);
        recv_len = strlen(buf);
        if(compare == 0){//une fois l'acceptation de connexion
            send_message(s,buf,recv_len,  (struct sockaddr *)&si_other, slen);
        }

        if(compare ==1){//acceptation de connexion
            compare = strcmp("SYN",buf);
            printf("compare vaut : %d\n",compare);
            if(compare==0){
                PORT2 ++;
                sprintf(message, "SYN-ACK_%d",PORT2);
                send_message(s,message,13,  (struct sockaddr *)&si_other, slen);                
                
                //FORK !!
                pid_fils = fork();

                if(pid_fils==0){
                    printf("FILS %d \n",getpid());
                    s = make_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
                    set_sockaddr_in(&si_me,PORT2,INADDR_ANY);
                    set_bind(s,&si_me, sizeof(si_me));                  
                }else{
                    printf("PÈRE \n");
                    compare = 1;
                    continue;					
                }                
            }
            else{
                send_message(s,"",0,  (struct sockaddr *)&si_other, slen);
                compare =1;
            }
        }
    }
    close(s);
    return 0;
}

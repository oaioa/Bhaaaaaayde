/*
   Simple udp client
http://www.binarytides.com/programming-udp-sockets-c-linux/
*/
#include "useful.h"

int main(void){
    struct sockaddr_in si_other;
    int s;
    int i;
    int slen=sizeof(si_other);
    int compare = 1 ; // il ya une difference
    int first = 0;

    char buf[BUFLEN];
    char message[BUFLEN];
    
    s = make_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    set_sockaddr_in(&si_other,PORT,0);

    if (inet_aton(SERVER , &si_other.sin_addr) == 0) {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
    while(1){
        if(compare == 0){ //connexion acceptée
            printf("Enter message : ");
            scanf("%s",message);
            send_message(s,message,strlen(message),  (struct sockaddr *)&si_other, slen);
        }
        if(compare ==1 ){ //processus d'acceptation de connexion	
            compare = string_compare(buf,strlen(buf)); //comparer le debut avec SYN-ACK
            if(compare==0){
                send_message(s,"ACK",3,  (struct sockaddr *)&si_other, slen);
                int PORT2 = atoi(index(buf,'_')+1);
                s =make_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
                set_sockaddr_in(&si_other,PORT2,0);
                if (inet_aton(SERVER , &si_other.sin_addr) == 0) 
                {
                    fprintf(stderr, "inet_aton() failed\n");
                    exit(1);
                }
                continue;
            }
            else{
                send_message(s,"SYN",3,  (struct sockaddr *)&si_other, slen);
                compare =1;
            }
        }
        receive_message(s, buf,NULL,0);
    }
    close(s);
    return 0;
}

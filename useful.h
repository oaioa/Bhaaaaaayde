#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>

#define SERVER "127.0.0.1"
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to send data

void die(char *s)
{
    printf("Adios...\n");
    perror(s);
    exit(1);
}

int delete(char buf[], int taille){
    int i = 0;	
    for(i = 0 ; i<taille ; i++){
        buf[i]='\0';
    }
    return 1;
}

//send the message
int send_message(int socket,char message[BUFLEN], size_t message_len,struct sockaddr *si_other,int si_other_len){

    if (sendto(socket, message, message_len , 0 , (struct sockaddr *) si_other,si_other_len)==-1)
    {
        die("sendto()");
        return 1;
    }
    return 0;
}

//receive the message
int receive_message(int socket, char buf[BUFLEN],struct sockaddr *si_other, int * si_other_len){
    memset(buf,'\0', BUFLEN);

    //try to receive some data, this is a blocking call
    if (recvfrom(socket, buf, BUFLEN, 0, (struct sockaddr *) si_other, si_other_len) == -1)
    {
        die("recvfrom()");
    }
    puts(buf);
}

int string_compare(char * s1, int size){ //size = strlen(buf);
	if (size <7){
		return -1;
	}else{
		int i = 0;
		char s2 [7] = "SYN-ACK";
		int same_letter = 0; //on suppose que la premiere lettre est ok
		do{
			if(s1[i] != s2[i])
				same_letter = -1;
			i++;
		}while(same_letter == 0 && i <7);
		return same_letter;
	}
}

//FRAGMENTAION
/*if ((f_in = fopen(ENTREE,"r")) == NULL)
            {
                fprintf(stderr, "\nErreur: Impossible de lire le fichier %s\n",ENTREE);
                return(EXIT_FAILURE);
            }
            if ((f_out = fopen(SORTIE,"w")) == NULL)
            {
                fprintf(stderr, "\nErreur: Impossible d'ecrire dans le fichier %s\n", \
                        SORTIE);
                return(EXIT_FAILURE);
            }
            while ((c = fgetc(f_in)) != EOF){
                delete(message,1024);
                message[counter_fragment]=c; 
                counter_fragment++;
                while (counter_fragment<1024 && ((c = fgetc(f_in)) != EOF) ){
                    message[counter_fragment]=c; 
                    counter_fragment++;
                }
                send_message(s,message,1026,  (struct sockaddr *)&si_other, slen);
                printf("J'ai envoyé %s \n",message);
                counter_fragment = 0;
            }
            fclose(f_in);
            fclose(f_out);
*/

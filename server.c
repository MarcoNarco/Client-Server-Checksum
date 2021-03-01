/*
    Marco Aguilera
    Simple udp server
*/
#include "header.h"

void die(char *s)
{
    perror(s);
    exit(1);
}

int main(int argc, char *argv[])
{
    SaccessHDR SUDPsegment;
    //SaccessHDR SudpConn;
    unsigned short int chksum_checking;
    unsigned short int sum = 0;
    unsigned short int sumfromClient = 0;
    unsigned short int totalCheck = 0;
    memset(&SUDPsegment, 0 , sizeof(SUDPsegment));

    struct sockaddr_in si_me, si_other;

    int sockfd, i, slen = sizeof(si_other) , recv_len, port;
    char buf[BUFLEN], tempS[2000], tempS2[BUFLEN]; char message[BUFLEN];
    int carry = 0;
    //usage: ./pserver <port number>
    if(argc != 2) {
        printf("usage: ./pserver <port number>\n");
        exit(1);
    }
    port = atoi(argv[1]);

    //create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }

    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));

    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(port);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind socket to port
    if(bind(sockfd, (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die("bind");
    }

/* Establishing connection that will help get the source port number */
    recvfrom(sockfd, tempS, sizeof(tempS), 0,(struct sockaddr *) &si_other, &slen);
    //printf("Test ip : %s\n", tempS); //Only for testing connection

    int sourcetemp = ntohs(si_other.sin_port); //Extracting the Source Number for the segment in client side
    snprintf(tempS2, sizeof(tempS2), "%d", sourcetemp); //Converting int to char array
    sendto(sockfd, tempS2, sizeof(tempS2), 0, (struct sockaddr *) &si_other , slen); //sending the source to the client
/* Recieving the segment from the client */
    recvfrom(sockfd, &sumfromClient, sizeof(sumfromClient), 0, (struct sockaddr *) &si_other , &slen);
    recvfrom(sockfd, &SUDPsegment, sizeof(SUDPsegment), 0, (struct sockaddr *) &si_other , &slen);
    memcpy(&SUDPsegment, &SUDPsegment, sizeof(SUDPsegment));
    chksum_checking = SUDPsegment.Schecksum;
    //printf("Sum from client: %u\n", sumfromClient);
    //printf("Stored Check from Client: %u\n", ~chksum_checking);

    SUDPsegment.Schecksum = 0;

    printf("Data segment from client:\n");
    //testing the print out to see if transmission works
    printf("source: %u\n", SUDPsegment.Ssource);
    printf("dest: %u\n", SUDPsegment.Sdestination);
    printf("length: %u\n", SUDPsegment.Slength);
    printf("checksum: %u\n", SUDPsegment.Schecksum);
    printf("payLOAD: %s\n", SUDPsegment.SpayLOAD);
    printf("===================\n");

/* Calculate the Checksum on Client side  */
    unsigned short int sizeSTRUCT = sizeof(SUDPsegment)/2; //size of my structure
    unsigned short int sixteenARRAY[sizeSTRUCT]; //16-bit array
    //memset(&sixteenARRAY, 0 , sizeof(sixteenARRAY));

    memcpy(sixteenARRAY, &SUDPsegment,sizeSTRUCT); //copying data into array

/* Setting up a forloop to add the content of the Array together */

    for(i = 0; i < sizeSTRUCT; i++)
    {
       carry = (sum + *message > 4294967295) ? 1 : 0;
        sum = sum + sixteenARRAY[i] + carry;
    }
    //printf("Sum: %u\n", sum);
    //printf("~Checksum: %u\n", ~sum);
    totalCheck = (~chksum_checking+sumfromClient);
    //printf("checkcheck: %u\n", totalCheck);
    printf("Sum: %u\n", sumfromClient);
    printf("Adding them together: %u\n", ~chksum_checking+sumfromClient);

    if(totalCheck == 65535)
    {
        printf(" Success!! Same!! No Errors!!\n");
        FILE *fp;
        fp = fopen("serverOUTPUT.txt","w");
        fprintf(fp, "%s\n", SUDPsegment.SpayLOAD);
        fclose(fp);
    }
    else{
        printf(" Failed!! NOT same\n");
    }

     FILE *fout;
    fout = fopen("server.out", "w");
    fprintf(fout, "AFTER CHECKSUM CALCULATION \n");
    fprintf(fout, "Source: %u\n", SUDPsegment.Ssource);
    fprintf(fout, "Destination: %u\n", SUDPsegment.Sdestination);
    fprintf(fout, "Length: %u\n", SUDPsegment.Slength);
    fprintf(fout, "Checksum: %u\n", ~chksum_checking);
    fprintf(fout, "PayLoad: %s\n", SUDPsegment.SpayLOAD);
    fprintf(fout, "==============\n");
    fprintf(fout, "Sum: %u\n", sumfromClient);
    //fprintf(fout, "Checksum: %u\n", SUDPsegment.Schecksum);

    fclose(fout);


    close(sockfd);
    return 0;
}

#ifndef HEADER_H
#define HEADER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>


#define SERVER "129.120.151.96"
#define BUFLEN 512  //Max length of buffer
#define PAYLOAD 1024
#define PORT 6700   //The port on which to send data



typedef struct dataSEG{
    unsigned short int source; //source port #
    unsigned short int destination; //destination port #
    unsigned short int receive; //receive window, set to 0
    unsigned short int checksum; //checksum to be computed after header and data is populated
    unsigned short int length;
    char payLOAD[PAYLOAD];

} accessHDR;

typedef struct SdataSEG{
    unsigned short int Ssource; //source port #
    unsigned short int Sdestination; //destination port #
    unsigned short int Sreceive; //receive window, set to 0
    unsigned short int Schecksum; //checksum to be computed after header and data is populated
    unsigned short int Slength;
    char SpayLOAD[PAYLOAD];

} SaccessHDR;



#endif


/*
    Marco Aguilera
    Simple udp client

*/

#include "header.h"

void die(char *s)
{
    perror(s);
    exit(1);
}

int main(int argc, char *argv[])
{

    accessHDR UDPsegment; // accessing the structure with this type

    //char tempIP[16]; //dont need
    unsigned short int S_port; //source number to start something
    unsigned short int payL;  //Im going to delete this and use the payload given
    unsigned short int l8bytes = 8; //This is just representing 8 bytes
    unsigned short int totLength; //Total length
    unsigned short int bufsize;
    unsigned short int sum = 0; int carry = 0; int n = 0;

    struct sockaddr_in si_other, sin;
    int sockfd, i=0, slen=sizeof(si_other), port, officialSource;
    int lenSource = sizeof(sin);
    char buf[BUFLEN], sourceTEST[BUFLEN] = {"3"}, sourTEMP[BUFLEN]; //but will hold the 1024 and the test will be nothing but a way to start connection
    char message[BUFLEN]; //only for the binary converting
    char openFile[20]; // allows the user to open the file
    char tempHold[PAYLOAD] = {'\0'}; //holds the of data from the file in a temporary moment
    char fullfiledata[PAYLOAD] = {'\0'}; //official data being held and represented
    char *buffer;


    if(argc != 3) {
        printf("usage: ./client <port number> <filename>\n"); //allows the user to put a port number and filename
        exit(1);
    }
    port = atoi(argv[1]); //port number converting to int from string
    strcpy(openFile,argv[2]); //filename that is being taken from command line
    //printf("File name: %s\n", openFile); //printing for testing purposes

    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(port);

    if (inet_aton(SERVER , &si_other.sin_addr) == 0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }


/*The file Opening startts here */

    FILE *fp; //enables file to be open
    fp = fopen(openFile,"rb");

      fseek(fp, 0, SEEK_END); //enables to run through the file to EOF
      bufsize = ftell(fp); //storing into a buffer
      fseek(fp, 0, SEEK_SET); //all the way to the EOF

         buffer = malloc(bufsize + 1); // adding one for the NULL
         fread(buffer, 1, bufsize, fp);
         //printf("Length bytes: %ld\n", bufsize); //checking the bytes
         //printf("PayLoad: %s\n", buffer); //checking the message theat will be transmitted

    fclose(fp);
    //printf("Length bytes: %ld\n", bufsize); //checking to see if the bytes come out of the else statement

/* Send a temp variable to start communication and take the Source Port Number  */

    sendto(sockfd, sourceTEST, sizeof(sourceTEST), 0, (struct sockaddr *) &si_other , slen); //forming connection
    payL = recvfrom(sockfd, sourTEMP, sizeof(sourTEMP), 0,(struct sockaddr *) &si_other, &slen); //recieving char array that holds the source port number
    //printf("Source Port # On Client Side : %s\n", sourTEMP); // source temp varibale for checking on what was recieved
    //printf("Payload : %u\n", payL); //size of the payload that is being sent through
    officialSource = atoi(sourTEMP);
    //printf("Integer Source Port Number %d\n", s); //this holds the official source port number.
    totLength = bufsize+l8bytes;

printf("Client Data BEFORE checksum:\n");

/* Populating all the data types in the segment */

    //groupSeg(&UDPsegment,port,officialSource,totLength,passCheckSum,buffer);
    UDPsegment.source = officialSource; // finding source number with function.
    UDPsegment.destination = port; //port number
    UDPsegment.length = totLength; //length of the something...
    UDPsegment.checksum = 0; //will be filled with a number after calculation
    //udpConn->payLOAD = b;
    strcpy(UDPsegment.payLOAD,buffer);
     printf("source: %u\n", UDPsegment.source);
     printf("dest: %u\n", UDPsegment.destination);
     printf("length: %u\n", UDPsegment.length);
     printf("checksum: %u\n", UDPsegment.checksum);
     printf("payLOAD: %s\n", UDPsegment.payLOAD);


/* Calculate the Checksum on Client side  */

    //create a 16bit array
    unsigned short int sizeSTRUCT = sizeof(UDPsegment)/2; //size of my structure
    unsigned short int sixteenARRAY[sizeSTRUCT]; //16-bit array

    memcpy(sixteenARRAY, &UDPsegment,sizeSTRUCT); //copying data into array

/* Setting up a forloop to add the content of the Array together */

     for(i = 0; i < sizeSTRUCT; i++)
     {
        carry = (sum + *message > 4294967295) ? 1 : 0;
         sum = sum + sixteenARRAY[i] + carry;
     }

     //printf("Carry: %d\n", carry);

     //unsigned short int passCheckSum = ~sum;


/* Populating all the data types in the segment */


    UDPsegment.checksum = sum; //will be filled with a number after calculation

/* Printing the final data types and calculation*/

printf("Client Data AFTER checksum:\n");
    printf("source: %u\n", UDPsegment.source);
    printf("dest: %u\n", UDPsegment.destination);
    printf("length: %u\n", UDPsegment.length);
    printf("checksum: %u\n", ~UDPsegment.checksum);
    printf("payLOAD: %s\n", UDPsegment.payLOAD);
    printf("===================\n");
    printf("Sum: %u\n", sum);
    printf("~Checksum: %u\n", ~sum);


    //memset(&UDPsegment, 0 , sizeof(UDPsegment));
/* Send segment over to server */
    sendto(sockfd, &sum, sizeof(sum), 0, (struct sockaddr *) &si_other , slen); //forming connection
    sendto(sockfd, (const void*)&UDPsegment, sizeof(UDPsegment)/2, 0, (struct sockaddr *) &si_other , slen);
    if(n == -1)
    {
        perror("Failed to send segment");
    }

/* Outputing the segment into a .out file but not the payload */
    FILE *fout;
   fout = fopen("client.out", "w");
   fprintf(fout, "AFTER CHECKSUM CALCULATION AND POPULATING\n");
   fprintf(fout, "Source: %u\n", UDPsegment.source);
   fprintf(fout, "Destination: %u\n", UDPsegment.destination);
   fprintf(fout, "Length: %u\n", UDPsegment.length);
   fprintf(fout, "Checksum: %u\n", ~UDPsegment.checksum);
   fprintf(fout, "PayLoad: %s\n", UDPsegment.payLOAD);
   fprintf(fout, "==============\n");
   fprintf(fout, "Sum: %u\n", sum);
   fprintf(fout, "Checksum: %u\n", ~sum);


    close(sockfd);
    return 0;

}

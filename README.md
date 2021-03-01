INSTRUCTION ON HOW TO COMPILE PROGRAM

NOTE TO GRADER:
ALL STEPS WERE COMPLETED

SERVER SIDE
COMPILATION RULES:

        NOTE:
            First make sure to touch the file just incase. Then go ahead and run "make"
            which can either prep the files for the port command or a message saying
            (Nothing to be done for 'all'.) will pop up.
            After that just run the ./server <PORT NUMBER>
            and then head to the client side terminal.

            The server will accept the segment and perform the
            calculation to check if the sum is the same on both sides once the calculation is all equal to 111111111111111111111111

    ***STEPS ARE BELOW ***

    1.   touch server.c
    2.   make
    3.   ./server <PORT NUMBER>
    4.   ****HEAD TO THE CLIENT SIDE****

CLIENT SIDE
COMPILATION RULES:

        NOTE:
            First make sure to touch the file just incase. Then go ahead and run "make"
            which can either prep the files for the port command or a message saying
            (Nothing to be done for 'all'.) will pop up.
            After that just run the ./server <PORT NUMBER> <filename>
            and the program is not in a loop so it will do ahead and compile once to keep
            the unique source port number.

            The client side will hold open the file the user inputs on command
            After the calculations are done the output is on the client.out file and on the console.
            Segment is sent to the server and lets the server
            take care of the rest of the program.


    ***STEPS ARE BELOW ***

    1.   touch client.c
    2.   make
    3.   ./client <PORT NUMBER> <filename>
    4.   ****PROGRAM BEGINS****

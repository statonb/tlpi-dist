/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2015.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 60-3 */

/* id_echo_cl3.c

   A client for the UDP "echo" service. This program sends each of its
   command-line arguments as a datagram to the server and echoes the
   contents of the datagrams that the server sends in response.

   See also id_echo_sv3.c.
*/
#include "id_echo.h"

int
main(int argc, char *argv[])
{
    int sfd;
    size_t len;
    ssize_t numRead;
    char buf[BUF_SIZE];
    char port[256];
    char ipAddress[256];
    char theMessage[256];
    int opt;
    int ipFlag = 0;
    int messageFlag = 0;
    int cmdErr = 0;

    strcpy(port, SERVICE);

    // ----------------------------------
    // Process command line arguments

    while ((opt = getopt(argc, argv, "h?p:i:m:")) != -1)
    {
        switch (opt)
        {
            case 'p':
                strcpy(port, optarg);
                break;
            case 'i':
                strcpy(ipAddress, optarg);
                ipFlag = 1;
                break;
            case 'm':
                strcpy(theMessage, optarg);
                messageFlag = 1;
                break;
            case '?':
            case 'h':
            default:
                cmdErr = 1;
                break;
        }
    }

    if  (   (1 == cmdErr)
         || (0 == ipFlag)
         || (0 == messageFlag)
        )
    {
        usageErr("%s -i host -m msg [-p port]\n", argv[0]);
        exit(-1);
    }

    /* Construct server address from first command-line argument */

    sfd = inetConnect(ipAddress, port, SOCK_DGRAM);
    if (sfd == -1)
        fatal("Could not connect to server socket");

    /* Send message to server */

    len = strlen(theMessage);
    if (write(sfd, theMessage, len) != len)
            fatal("partial/failed write");

    numRead = read(sfd, buf, BUF_SIZE);
    if (numRead == -1)
        errExit("read");

    printf("[%ld bytes] %.*s\n", (long) numRead, (int) numRead, buf);

    exit(EXIT_SUCCESS);
}

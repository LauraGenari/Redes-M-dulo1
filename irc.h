#ifndef _IRC_H_
#define _IRC_H_

#define BUFFER_SIZE 4096
#define DEBUG_MODE 0

void error(char *msg)
    {
        perror(msg);
        exit(1);
    }

#endif
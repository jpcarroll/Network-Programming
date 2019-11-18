/*
Author: jcarro5, Jack Carroll
Filename: l3.c
Description: This file provides the implementation of Layer 3.
*/

#include "l2.h"
#include "l3.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

const int chunkSize = 11; //max size of chunk can be <= 10

int l3_write(char *msg, int len) {

    int ret = 0, size = chunkSize - 1;
    char buff[chunkSize];

    //check passed in length is valid
    if (len < 1) { return -1; }
    else {
        //copy from msg to buff of max chunk size and send
        strncpy(buff, msg, size);
        buff[size] = '\0';
        ret = l2_write(buff, size);

        //check for error
        if (ret == -1) { return -1; }
        
        //reset buffer and keep sending chunks increasing size 
        //to offset what part of msg goes into buffer
        while (ret != -1 && size < len) {
            memset(buff, 0, sizeof(buff));
            strncpy(buff, msg + size, chunkSize - 1);
            ret = l2_write(buff, chunkSize - 1);
            size = size + 10;

            if (ret != -1) { ret = len; }
        }
    }

    return ret; 
}


int l3_read(char *msg, int max) { 

    int ret = 0, old = 0;
    char buff[chunkSize];
    memset(buff, 0 , sizeof(buff));
    //receive chunk and check for errors
    ret = l2_read(buff, chunkSize - 1);
    if (ret == -1 || ret >= max) { return -1; }
    else { 
        //copy buffer into msg, keeping track of size and reseting buffer
	strncpy(msg, buff, ret);
        old += ret;
        memset(buff, 0, sizeof(buff));
    }

    //receive chunk and check for errors, concatenating chunk into msg at the right position
    //and reseting buffer
    while (ret != -1 && old < max) {
        ret = l2_read(buff, chunkSize - 1); // <---- error is on this line
        if (max < ret || ret == -1) { return strlen(msg)+1; } //was return -1
        strncpy(msg + old, buff, ret);
        old += ret;
        memset(buff, 0, sizeof(buff));
    }

    //returns sum of the chunks' sizes
    return old;
}

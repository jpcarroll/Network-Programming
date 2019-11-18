/*
Author: jcarro5, Jack Carroll
Filename: l4.c
Description: This file provides the implementation of Layer 4.
*/

#include "l3.h"
#include "l4.h"
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

int l4_write(char *msg, int len) {
    
    //temp buffer with len + 1 to allow for checksum value
    char buff[len + 1];

    //calculate checksum, store in buffer, 
    //concatenate rest of msg to buff and send
    char check = (char) checksum(msg, len);
    buff[0] = check;
    strncpy(buff + 1, msg, len);
    int ret = l3_write(buff, len + 1);

    return (ret == -1)? -1: ret;
}

int l4_read(char *msg, int max) {

    //temp buffer with len + 1 to allow for checksum value
    char buff[max];
    memset(buff, 0, sizeof(buff));
    //receive message, check for errors
    int ret = l3_read(buff, max);
    if (ret == -1 || ret >= max) { return -1; }
    else if(buff[0] != (char) checksum(buff+1, strlen(buff+1))) { return -1; }
    else { 
	strcpy (msg , buff+1);
	return ret; 
	}
}

//calculates a one byte checksum with simple
//implementation from hw5 document
uint8_t checksum(char *msg, int len) {

    unsigned int sum;
    for (int i = len; i > 0; i--) {
        sum += *(msg++);
    }
    return ((uint8_t) (sum % 256));
}

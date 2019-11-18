#ifndef l4
#define l4

#include <stdint.h>

int l4_write(char *msg, int len);
int l4_read(char *msg, int max);
uint8_t checksum(char *msg, int len);

#endif

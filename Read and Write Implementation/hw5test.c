
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "l1.h"
#include "l2.h"
#include "l3.h"
#include "l4.h"

int main(int argc,char **argv) {

  char buff[4000];
  char inbuff[4000];
  int len;
  int err=0;
  int errcount=0;
  int i;
  int test;

  if (argc == 1) {
    test = 0;
  }
  else {
    test = atoi (argv[1]);
  }

  if (! test || test == 1) {  
    // try a small string
    init_l1(); err=0;
    printf("** l3 small string test: ");
    if (l3_write("Hello",6)==-1) {
  	printf("failed: write error\n"); err=1;
    } else {
  	len = l3_read(buff,1000);
  	if (len==-1) {
  	  printf("failed: read error. len = %d\n", len); err=1;
  	} else {
  	  if (len != 6) {
  		printf("failed: read returned wrong length\n"); err=1;
  	  } else {
  		if (strncmp("Hello",buff,6)!=0) {
  		  printf("failed: message is not right\n"); err=1;
  		} else {
  		  printf("OK\n");
  		}
  	  }
  	}
    }
    errcount+=err;
  }

  if (! test || test == 2) {  
    // try a large string
    memset(inbuff,'H',1000);
    inbuff[1000]=0;
    init_l1(); err=0;
    printf("** l3 long string test: ");
    if (l3_write(inbuff,1001)==-1) {
  	printf("failed: write error\n"); err=1;
    } else {
  	len = l3_read(buff,2000);
  	if (len==-1) {
  	  printf("failed: read error\n"); err=1;
  	} else {
  	  if (len != 1001) {
  		printf("failed: read returned wrong length\n"); err=1;
  	  } else {
  		if (strncmp(inbuff,buff,1001)!=0) {
  		  printf("failed: message is not right\n"); err=1;
  		} else {
  		  printf("OK\n");
  		}
  	  }
  	}
    }
    errcount+=err;
  }    
  
  if (! test || test == 3) {  
    // force read buffer to be too small
    // check for overflow!
    for (i=0;i<100;i++) {
  	buff[i]=0;
  	inbuff[i]=-1;
    }
  
    init_l1(); err=0;
    printf("** l3 message too big for buffer test: ");
    if (l3_write(inbuff,100)==-1) {
  	printf("failed: write error\n"); err=1;
    } else {
  	len = l3_read(buff,20);
  	if (len!=-1) {
  	  printf("failed: l3 read didn't return error\n");
  	  err=1;
  	} else {
  	  printf("OK\n");
  	}
  	if (buff[20]!=0) {
  	  printf("BUFFER OVERFLOW! (bytes modified)\n");
          printf ("\n!! Grading Test Result : Layer 4 test case 3 : Buffer overflow.\n");
  	}
    }
  }
  
  if (! test || test == 4) {  
    // force write error in layer 3
    init_l1(); err=0;
    printf("** l3 forced write error: ");
    generror=1;
    if (l3_write(inbuff,100)!=-1) {
  	printf("failed: no error returned\n"); err=1;
    } else {
  	printf("OK\n");
    }
    errcount+=err;
 
  }
  
  if (! test || test == 5) {  
    // force read error in layer 3
    init_l1(); err=0;
    printf("** l3 forced read error: ");
    generror=2;
    if (l3_write(inbuff,100)==-1) {
  	printf("failed: can't get l3_write to work\n"); err=1;
    } else {
  	len = l3_read(buff,100);
  	if (len!=-1) {
  	  printf("failed: no error returned\n"); err=1;
  	} else {
  	  printf("OK\n");
  	}
    }
    errcount+=err;

  }

  if (! test || test == 6) {  
    // try a small string
    init_l1(); err=0;
    printf("** l4 small string test: ");
    if (l4_write("Hello",6)==-1) {
  	printf("failed: write error\n"); err=1;
    } else {
  	len = l4_read(buff,1000);
  	if (len==-1) {
  	  printf("failed: read error\n"); err=1;
  	} else {
  	  if (len != 6) {
  		printf("failed: read returned wrong length\n"); err=1;
  	  } else {
  		if (strncmp("Hello",buff,6)!=0) {
  		  printf("failed: message is not right\n"); err=1;
  		} else {
  		  printf("OK\n");
  		}
  	  }
  	}
    }
    errcount+=err;
  }

  if (! test || test == 7) {  
    // try a large string
    memset(inbuff,'H',1000);
    inbuff[1000]=0;
    init_l1(); err=0;
    printf("** l4 long string test: ");
    if (l4_write(inbuff,1001)==-1) {
  	printf("failed: write error\n"); err=1;
    } else {
  	len = l4_read(buff,2000);
  	if (len==-1) {
  	  printf("failed: read error\n"); err=1;
  	} else {
  	  if (len != 1001) {
  		printf("failed: read returned wrong length\n"); err=1;
  	  } else {
  		if (strncmp(inbuff,buff,1001)!=0) {
  		  printf("failed: message is not right\n"); err=1;
  		} else {
  		  printf("OK\n");
  		}
  	  }
  	}
    }
    errcount+=err;
  }    
  
  if (! test || test == 8) {  
    // force read buffer to be too small
    // check for overflow!
    for (i=0;i<100;i++) {
  	buff[i]=0;
  	inbuff[i]=-1;
    }
  
    init_l1(); err=0;
    printf("** l4 message too big for buffer test: ");
    if (l4_write(inbuff,100)==-1) {
  	printf("failed: write error\n"); err=1;
    } else {
  	len = l4_read(buff,20);
  	if (len!=-1) {
  	  printf("failed: l4 read didn't return error\n");
  	  err=1;
  	} else {
  	  printf("OK\n");
  	}
  	if (buff[20]!=0) {
  	  printf("BUFFER OVERFLOW! (bytes modified)\n");
          printf ("\n!! Grading Test Result : Layer 4 test case 3 : Buffer overflow.\n");
  	}
    }
  }
  
  if (! test || test == 9) {  
    // force write error in layer 3
    init_l1(); err=0;
    printf("** l4 forced write error: ");
    generror=1;
    if (l4_write(inbuff,100)!=-1) {
  	printf("failed: no error returned\n"); err=1;
    } else {
  	printf("OK\n");
    }
    errcount+=err;
 
  }
  
  if (! test || test == 10) {  
    // force read error in layer 3
    init_l1(); err=0;
    printf("** l4 forced read error: ");
    generror=2;
    if (l4_write(inbuff,100)==-1) {
  	printf("failed: can't get l4_write to work\n"); err=1;
    } else {
  	len = l4_read(buff,100);
  	if (len!=-1) {
  	  printf("failed: no error returned\n"); err=1;
  	} else {
  	  printf("OK\n");
  	}
    }
    errcount+=err;

  }
  
  if (! test || test == 11) {  
    // Finally - the good stuff
    // mess with the checksum!
    init_l1(); err=0;
    for (i=0;i<100;i++) {
  	buff[i]=0;
  	inbuff[i]=i;
    }
  
    printf("** l4 Checksum error: ");
    generror=5;
    if (l4_write(inbuff,100)==-1) {
  	printf("failed: can't get l4_write to work\n"); err=1;
    } else {
  	len = l4_read(buff,100);
  	if (len!=-1) {
  	  printf("failed: no error returned\n"); err=1;
  	} else {
  	  printf("OK\n");
  	}
    }
    errcount+=err;
  }
  

}



      

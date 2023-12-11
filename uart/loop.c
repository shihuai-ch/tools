#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<strings.h>

int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
	struct termios newtio,oldtio;
	if( tcgetattr( fd,&oldtio)  !=  0) {
	    perror("tcgetattr error");
	    return -1;
	}
	bzero( &newtio, sizeof( newtio ) );
	newtio.c_cflag  |=  CLOCAL | CREAD; 
	newtio.c_cflag &= ~CSIZE;

    //newtio.c_lflag &=~ICANON;
    //newtio.c_lflag |=ICANON;
	
    switch( nBits )
	{
	    case 7:
	        newtio.c_cflag |= CS7;
	        break;
	    case 8:
	        newtio.c_cflag |= CS8;
	        break;
	}

	switch( nEvent )
	{
	    case 'O':
	        newtio.c_cflag |= PARENB; 
	        newtio.c_cflag |= PARODD;  
	        newtio.c_iflag |= (INPCK | ISTRIP); 
	        break;
	    case 'E':
            newtio.c_iflag |= (INPCK | ISTRIP);
            newtio.c_cflag |= PARENB;
            newtio.c_cflag &= ~PARODD;
            break;
	    case 'N': 
	        newtio.c_cflag &= ~PARENB;
	        break;
	}

	switch( nSpeed )
	{
	    case 2400:
	        cfsetispeed(&newtio, B2400);
	        cfsetospeed(&newtio, B2400);
	        break;
	    case 4800:
	        cfsetispeed(&newtio, B4800);
	        cfsetospeed(&newtio, B4800);
	        break;
	    case 9600:
	        cfsetispeed(&newtio, B9600);
	        cfsetospeed(&newtio, B9600);
	        break;
	    case 115200:
	        cfsetispeed(&newtio, B115200);
	        cfsetospeed(&newtio, B115200);
	        break;
	    case 460800:
	        cfsetispeed(&newtio, B460800);
	        cfsetospeed(&newtio, B460800);
	        break;
			case 500000:
	        cfsetispeed(&newtio, B500000);
	        cfsetospeed(&newtio, B500000);
	        break; 
			case 576000:
	        cfsetispeed(&newtio, B576000);
	        cfsetospeed(&newtio, B576000);
	        break; 
			case 921600:
	        cfsetispeed(&newtio, B921600);
	        cfsetospeed(&newtio, B921600);
	        break;   
	    case 1000000:
	        cfsetispeed(&newtio, B1000000);
	        cfsetospeed(&newtio, B1000000);
	        break; 
	    case 1152000:
	        cfsetispeed(&newtio, B1152000);
	        cfsetospeed(&newtio, B1152000);
	        break;
	    default:
	        cfsetispeed(&newtio, B9600);
	        cfsetospeed(&newtio, B9600);
	        break;
	}

	if( nStop == 1){
	    newtio.c_cflag &=  ~CSTOPB; 
	}else if ( nStop == 2 ){
	    newtio.c_cflag |=  CSTOPB;
	} 
	newtio.c_cc[VTIME]  = 0;
	newtio.c_cc[VMIN] = 0;
	tcflush(fd,TCIFLUSH); 
	if((tcsetattr(fd,TCSANOW,&newtio))!=0)
	{
	    perror("set error");
	    return -1;
	}
	return 0;
}

int main(int argc,char *argv[])
{
	int fd,ret_set,ret_read,ret;
	char buf_read[1024];
	char buf_write[1024];
	char tty[20]="/dev/";
	if(3 < argc)
	{
	    strcat(tty,argv[1]);
	    fd = open(tty, O_RDWR);
	    if(fd == -1)
	    {
				printf("Open %s failed! Exit!\n",tty);
				exit(1);
	    }
	    printf("open %s successfully!\n",tty);

	    ret_set = set_opt(fd, atoi(argv[2]), 7, 'O', 1);
	    if (ret_set == -1)
	    {
				printf("Set %s failed! Exit!\n",tty);
				exit(1);
	    }
	    printf("Set %s successfully!\n",tty);
	    printf("Baud rate: %s\n",argv[2]);
	    memset(buf_write, 0, sizeof(buf_write));	
	    memcpy(buf_write,argv[3],sizeof(buf_write));
	    buf_write[99] = '\n';
			printf("Data: %s, size: %lu\n",buf_write,strlen(buf_write));
	    while (1)
	    { 
	        memset(buf_read, 0, sizeof(buf_read));
	        ret = write(fd, buf_write, strlen(buf_write)+1);
					if( ret > 0){
		   			printf("Write data: %s\n",buf_write);
	        }else{
				    printf("Write data failed! Exit!\n");
				    exit(1);
					}
		    usleep(2*1000);
			
	        ret_read = read(fd, buf_read, 100);
	        if(ret_read > 0){
	            printf("Read data: %s\n\n", buf_read);
	        }
	        sleep(1);
	    }
			close(fd);
	}else{
	    printf("Usage: uart [tty node] [baud rate] [data] [data size < 1024]\n");
	    printf("Sample: uart ttyHSL1 115200 test\n");
	}
 return 0;
}



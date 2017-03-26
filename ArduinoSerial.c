#include <jni.h>
#include <stdio.h>    /* Standard input/output definitions */
#include <stdlib.h> 
#include <stdint.h>   /* Standard types */
#include <string.h>   /* String function definitions */
#include <unistd.h>   /* UNIX standard function definitions */
#include <fcntl.h>    /* File control definitions */
#include <errno.h>    /* Error number definitions */
#include <termios.h>  /* POSIX terminal control definitions */
#include <sys/ioctl.h>
#include <getopt.h>

int serialport_init(const char* serialport, int baud);
int serialport_read_until(int fd, char* buf, char until);
JNIEXPORT jstring JNICALL Java_ArduinoSerial_readTemperature
  (JNIEnv *env, jobject obj, jstring str, jint i){
	jstring _str = (jstring)malloc(sizeof(jstring));
	char buf[256];
	//handle para
	char const *s=(*env)->GetStringUTFChars(env,str,NULL);//convert to ctype
	int baud = i;
	int fd = serialport_init(s,baud);//open serialport
	serialport_read_until(fd,buf,'\n');//read to buffer
	//printf("value read by c is:%s\n",buf);
	(*env)->ReleaseStringUTFChars(env,str,s);//realease the args
	/*int cnt=0;
	for(;cnt<256;cnt++){
	    printf("%d\t",buf[cnt]);	
	}*/
	return (*env)->NewStringUTF(env,buf);

}
int serialport_init(const char* serialport, int baud)
{
    struct termios toptions;
    int fd;
    
    //fprintf(stderr,"init_serialport: opening port %s @ %d bps\n",
    //        serialport,baud);

    fd = open(serialport, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)  {
        perror("init_serialport: Unable to open port ");
        return -1;
    }
    
    if (tcgetattr(fd, &toptions) < 0) {
        perror("init_serialport: Couldn't get term attributes");
        return -1;
    }
    speed_t brate = baud; // let you override switch below if needed
    switch(baud) {
    case 4800:   brate=4800;   break;
    case 9600:   brate=9600;   break;
    case 14400:  brate=14400;  break;
    case 19200:  brate=19200;  break;
    case 28800:  brate=28800;  break;
    case 38400:  brate=38400;  break;
    case 57600:  brate=57600;  break;
    case 115200: brate=115200; break;
    }
    cfsetispeed(&toptions, brate);
    cfsetospeed(&toptions, brate);

    // 8N1
    toptions.c_cflag &= ~PARENB;
    toptions.c_cflag &= ~CSTOPB;
    toptions.c_cflag &= ~CSIZE;
    toptions.c_cflag |= CS8;
    // no flow control
    toptions.c_cflag &= ~CRTSCTS;

    toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
    toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl

    toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    toptions.c_oflag &= ~OPOST; // make raw

    // see: http://unixwiz.net/techtips/termios-vmin-vtime.html
    toptions.c_cc[VMIN]  = 0;
    toptions.c_cc[VTIME] = 20;
    
    if( tcsetattr(fd, TCSANOW, &toptions) < 0) {
        perror("init_serialport: Couldn't set term attributes");
        return -1;
    }

    return fd;
}
//read to buf
int serialport_read_until(int fd, char* buf, char until)
{
    char b[1];
    int i=0;
    do { 
        int n = read(fd, b, 1);  // read a char at a time
        if( n==-1) return -1;    // couldn't read
        if( n==0 ) {
            usleep( 10 * 1000 ); // wait 10 msec try again
            continue;
        }
        buf[i] = b[0]; i++;
    } while( b[0] != until );
    printf("value in c:%s",buf);
    buf[i] = 0;  // null terminate the string
    return 0;
}


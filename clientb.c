/*
* Programe is written in order to determine the overhead
* of running a program in FreeBSD on capability mode.
* The program  as part the project carried out to fullfill the
* one of the requirements of ENG_9875 Real-Time Operating system course
*
* Written by
* Stanley Uche Godfrey
* March 10,2016 
*/
#include<stdlib.h>
#include<stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>
#include<sys/capability.h>
#include<err.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include"fileDes.h"
#include<sys/time.h>
// A method for receiving file descriptor
static int
recv_file_descriptor(
  int socket) /* Socket from which the file descriptor is read */
{
 int sent_fd;
 struct msghdr message;
 struct iovec iov[1];
 struct cmsghdr *control_message = NULL;
 char ctrl_buf[CMSG_SPACE(sizeof(int))];
 char data[1];
 int res;

 memset(&message, 0, sizeof(struct msghdr));
 memset(ctrl_buf, 0, CMSG_SPACE(sizeof(int)));

 /* For the dummy data */
 iov[0].iov_base = data;
 iov[0].iov_len = sizeof(data);

 message.msg_name = NULL;
 message.msg_namelen = 0;
 message.msg_control = ctrl_buf;
 message.msg_controllen = CMSG_SPACE(sizeof(int));
 message.msg_iov = iov;
 message.msg_iovlen = 1;

 if((res = recvmsg(socket, &message, 0)) <= 0)
  return res;

 /* Iterate through header to find if there is a file descriptor */
 for(control_message = CMSG_FIRSTHDR(&message);
     control_message != NULL;
     control_message = CMSG_NXTHDR(&message,
                                   control_message))
 {
  if( (control_message->cmsg_level == SOL_SOCKET) &&
      (control_message->cmsg_type == SCM_RIGHTS) )
  {
   return *((int *) CMSG_DATA(control_message));
  }
 }

 return -1;
}



 int main(int argc,char* argv[]){
	struct timespec start_time,end_time;
	int sockfd;
	int len;
	struct sockaddr_un address;
	int result;
	int i,b,c,d,n,pfd;
	char readWords[128];
	char uppercase[128];
	char* newline="\n\n";
	char buffer[128];
	clock_gettime(CLOCK_REALTIME,&start_time); // Save the time program starts executing
	// Initallizing my Struct named SocketMessage
	SocketMessage *sMsg= (SocketMessage *)malloc(sizeof(SocketMessage));
	if(argc!=3){
	  /*printf("More commandline arguments needed !\n");
	  printf("Start the server (server program) and then \n");
	  printf("Include the files to read from and write to as commandline arguments\n");
	  printf("When you start the server program\n");*/
	  exit(0);
	}
	
	strcpy(sMsg->rdFileName,argv[1]);//copy file to read from from command line
	strcpy(sMsg->wrFileName,argv[2]);//copy file to write to from the command line
	
	sockfd= socket(AF_UNIX,SOCK_STREAM,0);
	address.sun_family=AF_UNIX;
	strcpy(address.sun_path,"s_socket");
	len=sizeof(address);
	result=connect(sockfd,(struct sockaddr*) &address,len);
	if(result== -1){
		perror("OOPS: Client cannot connect \n");
		exit(1);
	}
	
	d=write(sockfd,sMsg->rdFileName,sizeof(sMsg->rdFileName));
	
	
	
	
	
	if(d>0 && c>0){
	  /*printf(" \n");
	  printf(" file name %s was sent to server successfully \n",sMsg->rdFileName);
	  printf(" \n");
	  printf(" ...............................\n");
	  printf(" \n");*/
	}
	sMsg->fileDesRd=recv_file_descriptor(sockfd);
	if(sMsg->fileDesRd<0)
			err(-1,"can't read from socket");
	
	d=write(sockfd,sMsg->wrFileName,sizeof(sMsg->wrFileName));
	
	
	
	if(d>0 && c>0){
	  /*printf(" \n");
	  printf("file name %s  was sent to server successfully \n",sMsg->wrFileName);
	  printf(" \n");
	  printf(" ...............................\n");
	  printf(" \n");*/
	}
	sMsg->fileDesWr=recv_file_descriptor(sockfd);
	if(sMsg->fileDesWr<0)
			err(-1,"can't read from socket");
	

	do{
		n=read(sMsg->fileDesRd,readWords,sizeof(readWords));
		if(n>0){
			for(i=0;i<n;i++){
				uppercase[i]=toupper(readWords[i]);
			}
			write(sMsg->fileDesWr,uppercase,n);
			write(sMsg->fileDesWr,newline,strlen(newline));
			}				
				
			
	}while(n>0);

	write(sMsg->fileDesWr,newline,strlen(newline));
		
	clock_gettime(CLOCK_REALTIME,&end_time);
	
	printf("Time cost without CAP_ENTER enabled in nano seconds is printed in execution_cost_b.txt file\n");
	pfd=open("execution_cost_b.txt",O_WRONLY|O_CREAT|O_APPEND);
	if(pfd<0){
		err(-1,"Error creating execution file");
	}
	sprintf(buffer,"%ld\t",end_time.tv_nsec-start_time.tv_nsec);
	write(pfd,buffer,strlen(buffer));
	write(pfd,newline,strlen(newline));
	close(sMsg->fileDesRd);
	close(sMsg->fileDesWr);
	close(sockfd);
	
	free(sMsg);
	
	return 0;	
}
	

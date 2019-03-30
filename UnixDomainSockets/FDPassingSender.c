#include "../cn.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define PATH "/tmp/fd_passing.socket"
#define CONTROLLEN CMSG_LEN(sizeof(int))


int main(int argc, char const *argv[])
{
	int usfd = socket(AF_UNIX,SOCK_STREAM,0);
	if(usfd<0)
	{
		perror("Could not create socket");exit(0);
	}
	struct sockaddr_un addr;
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path,PATH);
	if(bind(usfd,(struct sockaddr*)&addr,sizeof(addr))<0)
		perror("Could not bind");
	else
		printf("Bind successful\n");
	if(listen(usfd,10)<0)
		perror("Could not listen");
	else
		printf("Listen successful\n");
	int nsfd = accept(usfd,NULL,NULL);
	if(nsfd<0)
		perror("Could not accept");
	int fd = open("file.txt",O_RDONLY);
	if(fd<0)
	{
		perror("Could not open");exit(0);
	}
	struct msghdr msg;
	char buffer[2];
	struct iovec iov[1];
	iov[0].iov_base = buffer;
	iov[0].iov_len = 2;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	struct cmsghdr *cmptr;
	cmptr = (struct cmsghdr*)malloc(CONTROLLEN);
	cmptr->cmsg_level = SOL_SOCKET;
	cmptr->cmsg_type = SCM_RIGHTS;
	cmptr->cmsg_len = CONTROLLEN;
	msg.msg_control = cmptr;
	msg.msg_controllen = CONTROLLEN;
	*(int*)CMSG_DATA(cmptr) = fd;
	if(sendmsg(nsfd,&msg,0)<0)
		perror("Could not send");
	else
		printf("Success..\n");
	unlink(PATH);
	return 0;
}
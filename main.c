#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<errno.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<signal.h>
#include<sys/wait.h>

int serverSock;

void printError(){
	char* errmes=strerror(errno);
	fprintf(stderr,"\nERROR:%s\n",errmes);
}

void die(){
	printError();
	exit(1);
}

void daemon_term(){
	close(serverSock);
	printf("Goodbye...\n");
	exit(0);
}

void daemon_sigchild(){
	int status;
	wait(&status);
}

void daemon_main(){
	int clientSock;
	struct sockaddr_in clientAddr;
	socklen_t clientAddr_len;

	while(1){
		clientSock = accept(
					serverSock,
					(struct sockaddr*)&clientAddr,
					(socklen_t*)&clientAddr_len
					);
		if(clientSock==-1){
			printError();
			continue;
		}
		if(!fork()){
			char mes[]="Hello World!!\r\n";
			int res=send(
				clientSock,
				mes,
				sizeof(mes),
				0
				);
			if(res==-1)printError();
			shutdown(clientSock,SHUT_RDWR);
			close(clientSock);
			exit(0);
		}
	}
}

int main(){
	int res;

	signal(SIGPIPE,SIG_IGN);
	signal(SIGTERM,daemon_term);
	signal(SIGCHLD,daemon_sigchild);

	serverSock=socket(AF_INET,SOCK_STREAM,0);
	if(serverSock==-1)die();

	{
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(4649);
		addr.sin_addr.s_addr = INADDR_ANY;
		res=bind(
				serverSock,
				(struct sockaddr*)&addr,
				sizeof(addr)
				);
		if(res==-1)die();
	}

	res=listen(serverSock,1);
	if(res==-1)die();

	if(!fork())daemon_main();
	return 0;
}

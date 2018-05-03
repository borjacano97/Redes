#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

//int socket(int domain, int type, int protocol);
int main (int argc, char** argv)
{
	char * hostName;
	if (argc > 1)
		hostName = argv[1];
	else return -1;
	
	hostent* node = gethostbyname(hostName);
	servent* service = getservbyname(hostName, NULL);
	struct addrinfo* res;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	
	int success = getaddrinfo(node->h_name, service->s_name, &hints, &res);
	if (success != 0) return success;
	
	for (struct addrinfo* p = res; p != NULL; p->ai_next)
	{
		char host[NI_MAXHOST];
		char serv[NI_MAXSERV];
		getnameinfo(p->ai_addr, p->ai_addrlen, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST);
		
		std::cout<< host << " " << p->ai_family << " "<< p->ai_socktype << std::endl;
	}
	
	freeaddrinfo(res);	
	
	return 0;	
};
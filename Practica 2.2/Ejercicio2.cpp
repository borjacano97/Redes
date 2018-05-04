#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <iostream>

int main (int argc, char** argv)
{
	if (argc <= 1) return -1;

	struct addrinfo* res;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	
	int success = getaddrinfo(argv[1], argv[2], &hints, &res);
	if (success != 0)
	{	
		std::cout << "ERROR: "<< gai_strerror(success)<<std::endl;
		return success;
	}
	
	for (struct addrinfo* p = res; p != NULL; p = p->ai_next)
	{
		char host[NI_MAXHOST];
		char serv[NI_MAXSERV];

		getnameinfo(p->ai_addr, p->ai_addrlen, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
		
		std::cout<< "Host: " << host << "\nServ" << serv << std::endl;
	}

	int sd = socket(res->ai_family, res->ai_socktype, 0);
	bind(sd, (struct sockaddr *)res->ai_addr, res->ai_addrlen);

	char buff[80];
	struct sockaddr src_addr;
	socklen_t addrlen = sizeof(src_addr);

	int bytesRecived = recvfrom(sd, buff, 80, 0, &src_addr, &addrlen);
	std::cout<<bytesRecived <<" bytes de " << src_addr.sa_family<< std::endl;
	
	freeaddrinfo(res);	
	
	return 0;	
};

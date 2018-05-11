
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <sstream>
#include <iostream>

int main (int argc, char** argv)
{
	if (argc <= 1) {
		std::cout << "Usange: ./time_client \"direction\" \"port\" \"command\"\nExample: ./time_server 0.0.0.0 3000 t" << '\n';
		return -1;
	};

	char command = *(argv[3]);
	struct addrinfo* res;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family   = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	int success = getaddrinfo(argv[1], argv[2], &hints, &res);
	if (success != 0)
	{
		std::cout << "ERROR: "<< gai_strerror(success)<<std::endl;
		return success;
	}

	int sd = socket(res->ai_family, res->ai_socktype, 0);

	char buff[80];
	struct sockaddr src_addr = *res->ai_addr;
	socklen_t addrlen = res->ai_addrlen;

	while (1){
		memset(buff, '\0', 80);
		buff[0] = command;
		sendto(sd, buff, strlen(buff), 0, &src_addr, addrlen);
		int bytesRecived = recvfrom(sd, buff, 80, 0, NULL, NULL);

		if (bytesRecived > 0) std::cout<< buff;
		std::cin>>command;
	}
	
	freeaddrinfo(res);

	return 0;
};


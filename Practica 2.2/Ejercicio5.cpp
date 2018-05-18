#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <ctime>

#define BUFFER_SIZE 256

int main (int argc, char** argv)
{
	if (argc <= 1) {
		std::cout << "Usange: ./eco_client \"direction\" \"port\"\nExample: ./eco_client 0.0.0.0 3000" << '\n';
		return -1;
	};

	struct addrinfo* res;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags		= AI_PASSIVE; //Devolver 0.0.0.0
	hints.ai_family   = AF_INET; //IPv4
	hints.ai_socktype = SOCK_STREAM;

	int success = getaddrinfo(argv[1], argv[2], &hints, &res);
	if (success != 0)
	{
		std::cout << "ERROR: "<< gai_strerror(success)<<std::endl;
		return success;
	}

	int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	char buffer[BUFFER_SIZE];
	struct sockaddr server_addr;
	socklen_t server_addrlen = sizeof(server_addr);
	connect(sd, res->ai_addr, res->ai_addrlen);

	//Recive msg and send it back to client
	std::string input;
	while (1){
		memset(buffer, '\0', BUFFER_SIZE);//Reset buffer
		std::cin >> input;
		/*if ((input[0] == 'q' || input[0] == 'Q') && input.length() < 2)
		{//Close conection
			close(sd);
			break;
		}*/
		input += '\n';
		send (sd, input.c_str(), input.length(), 0);
		recv(sd, buffer, BUFFER_SIZE, 0);
		std::cout << buffer << '\n';
	}
	std::cout<<"Saliendo...\n";
	freeaddrinfo(res);

	return 0;
};

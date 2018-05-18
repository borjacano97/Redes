
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <sstream>
#include <iostream>
#include <ctime>

#define BUFFER_SIZE 256

int main (int argc, char** argv)
{
	if (argc <= 1) {
		std::cout << "Usange: ./eco_server \"direction\" \"port\"\nExample: ./eco_server 0.0.0.0 3000" << '\n';
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

	int sd = socket(res->ai_family, res->ai_socktype, 0);
	bind(sd, (struct sockaddr *)res->ai_addr, res->ai_addrlen);

	listen (sd, 5);
	char buffer[BUFFER_SIZE];
	struct sockaddr src_addr, client_addr;
	socklen_t src_addrlen = sizeof(src_addr);
	socklen_t client_addrlen = sizeof(client_addr);
	//Print conection info
	char host[NI_MAXHOST];
	char serv[NI_MAXSERV];

	int client_sd = accept(sd, (struct sockaddr*) &client_addr, &client_addrlen);
	std::cout << "Accept done!" << '\n';
	getnameinfo(&client_addr, client_addrlen, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
	std::cout << "Conexion desde " << host << " "<< serv << '\n';

	//Recive msg and send it back to client
	while (1){
		memset(buffer, '\0', BUFFER_SIZE);//Reset buffer
		std::cout << "buffer reseted" << '\n';
		int msg_size = 0;
		int bytesRecived;
		do {//Fill the buffer until buffer isn't full or haven't end the msg and still getting bytes
			bytesRecived = recv(client_sd, &(buffer[msg_size]), 1, 0);
		} while(bytesRecived >= 0 && msg_size < BUFFER_SIZE - 1 && buffer[msg_size++] != '\n');

		//Sent to client the buffer
		send(client_sd, buffer, msg_size, 0);
	}
	std::cout<<"Saliendo...\n";
	freeaddrinfo(res);

	return 0;
};

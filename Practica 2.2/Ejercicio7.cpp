
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <sstream>
#include <iostream>
#include <ctime>
#include <unistd.h>

#define BUFFER_SIZE 256

class ServerThread {
private:
	int sd;

public:
	ServerThread(int s): sd(s){};
	virtual ~ServerThread(){};

	void do_message()
	{
		bool exit = false;
		char buffer[255];
		struct sockaddr src_addr;
		socklen_t addrlen = sizeof(src_addr);

		size_t s;
		while (!exit){
			memset(buffer, '\0', BUFFER_SIZE);//Reset buffer
			int msg_size = 0;
			int bytesRecived;
			std::string aux = "\rReciving bytes";
			do {//Fill the buffer until buffer isn't full or haven't end the msg and still getting bytes
			std::cout << (aux += ".");
				bytesRecived = recv(sd, &(buffer[msg_size]), 1, 0);
			} while(bytesRecived >= 0 && msg_size < BUFFER_SIZE - 1 && (buffer[msg_size++] != '\n' || buffer[msg_size++] != '\0') );
			std::cout << "\nSending msg back!\n";
			//Sent to client the buffer
			send(sd, buffer, msg_size, 0);
			sleep(3);
			}
		}
};

extern "C" void* start_routine(void* _st)
{
	ServerThread* st = static_cast<ServerThread*> (_st);
	st->do_message();
	return 0;
}


int main (int argc, char** argv)
{
	if (argc <= 1) {
		std::cout << "Usange: ./eco_server_thr \"direction\" \"port\"\nExample: ./eco_server_thr 0.0.0.0 3000" << '\n';
		return -1;
	};

	struct addrinfo* res;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags		= AI_PASSIVE; //Devolver 0.0.0.0
	hints.ai_family   = AF_INET; //IPv4
	hints.ai_socktype = SOCK_STREAM;//TCP

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

	while (1){
			int client_sd = accept(sd, (struct sockaddr*) &client_addr, &client_addrlen);
			std::cout << "Accept done!" << '\n';
			getnameinfo(&client_addr, client_addrlen, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
			std::cout << "Conexion desde " << host << " "<< serv << '\n';

			pthread_t tid;
			pthread_attr_t attr;
			ServerThread* st = new ServerThread(client_sd);

			pthread_attr_init(&attr);
			pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
			pthread_create(&tid, &attr, start_routine, static_cast<void*>(st));

	}
	std::cout<<"Saliendo...\n";
	freeaddrinfo(res);

	return 0;
};

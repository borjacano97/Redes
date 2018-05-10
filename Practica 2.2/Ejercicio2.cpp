
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <iostream>

#include <ctime>
void getHour(char* target)
{
	time_t rawtime;
	tm * timeinfo;

	time (&rawtime);
	timeinfo = localtime (&rawtime);
	const char * result = (std::to_string(timeinfo->tm_hour) +  " : " + std::to_string(timeinfo->tm_min) +  " : " + std::to_string(timeinfo->tm_sec) + "\n").c_str();
	strcpy(target, result);
}
void getDate(char* target)
{
	time_t rawtime;
	tm * timeinfo;

	time (&rawtime);
	timeinfo = localtime (&rawtime);
	const char* result =  (std::to_string(timeinfo->tm_year + 1900) + " - "+ std::to_string(timeinfo->tm_mon +1 ) +  " - " +  std::to_string(timeinfo->tm_mday) + "\n").c_str();
	strcpy(target, result);
}

int main (int argc, char** argv)
{
	if (argc <= 1) {
		std::cout << "Usange: ./time_server \"direction\" \"port\"\nExample: ./time_server 0.0.0.0 3000" << '\n';
		return -1;
	};

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
/*
	for (struct addrinfo* p = res; p != NULL; p = p->ai_next)
	{
		char host[NI_MAXHOST];
		char serv[NI_MAXSERV];

		getnameinfo(p->ai_addr, p->ai_addrlen, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

		std::cout<< "Host: " << host << "\nServ: " << serv << std::endl;
	}
*/
	int sd = socket(res->ai_family, res->ai_socktype, 0);
	bind(sd, (struct sockaddr *)res->ai_addr, res->ai_addrlen);

	char buff[80];
	struct sockaddr src_addr;
	socklen_t addrlen = sizeof(src_addr);

	while (1){
		int bytesRecived = recvfrom(sd, buff, 80, 0, &src_addr, &addrlen);
		if (bytesRecived > 0)
		{
			std::cout<< bytesRecived <<" bytes de " << src_addr.sa_family<< std::endl;
			if (buff[0] == 'q') break;
			if (buff[0] == 'd') getHour(&(buff[0]));
			else if (buff[0] == 't')getDate(&(buff[0]));
			else std::cout<<"Comando no soportado: "<< buff[0]<< "\n";

			buff[bytesRecived] = '\0';
			sendto(sd, buff, 80, 0, (sockaddr*) &src_addr, addrlen);
		}
	}
	std::cout<<"Saliendo...\n";
	freeaddrinfo(res);

	return 0;
};



#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <pthread.h>

#include <ctime>
unsigned int NUM_THREADS = 4	;
static void getHour(char* target)
{
	time_t rawtime;
	tm * timeinfo;

	time (&rawtime);
	timeinfo = localtime (&rawtime);

	std::ostringstream oss;

	oss << timeinfo->tm_hour << " : " << timeinfo->tm_min << " : " << timeinfo->tm_sec << "\n";

	strcpy(target, oss.str().c_str());
}
static void getDate(char* target)
{
	time_t rawtime;
	tm * timeinfo;

	time (&rawtime);
	timeinfo = localtime (&rawtime);
	std::ostringstream oss;
	oss << timeinfo->tm_year + 1900 << " - "<<timeinfo->tm_mon +1 << " - " << timeinfo->tm_mday << "\n";
	//const char* result =  (std::to_string(timeinfo->tm_year + 1900) + " - "+ std::to_string(timeinfo->tm_mon +1 ) +  " - " +  std::to_string(timeinfo->tm_mday) + "\n").c_str();
	strcpy(target, oss.str().c_str());
}

class ServerThread {
private:
	int sd;

	void parseAndTreat(char option, char* buffer_out, bool &exit)
	{
		switch (option) {
			case 't'://get Time
				getHour(buffer_out);
			break;
			case 'd'://gat Date
			getDate(buffer_out);
			break;
			case 'q'://Quit
			exit = true;
			break;
			default:
				std::cout << "Comando \"" << option << "\" no soportado!" << '\n';
			break;
		}
	}

public:
	ServerThread(int s): sd(s){};
	virtual ~ServerThread(){};

	void do_message()
	{
		bool exit = false;
		char buf[255];
		struct sockaddr src_addr;
		socklen_t addrlen = sizeof(src_addr);

		size_t s;
		while (!exit){
			memset(buf,'\0', 255);
			s = recvfrom(sd, buf, 255, 0, &src_addr, &addrlen);
			std::cout << "s:"<< s << '\n';
			std::cout << "Msg recived!" << '\n';
			std::cout << "Thread: " << pthread_self() << "\n";
			parseAndTreat(buf[0], buf, exit);
			sendto(sd, buf, 255, 0, &src_addr, addrlen);
			sleep(60);
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
		std::cout << "Usange: ./time_server_threaded \"direction\" \"port\"\nExample: ./time_server_threaded 0.0.0.0 3000" << '\n';
		return -1;
	};

	struct addrinfo* res;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family   = AF_INET;//IPv4
	hints.ai_socktype = SOCK_DGRAM;//UDP
	std::cout << "Starting server..." << '\n';
	//Get ip and port information
	int success = getaddrinfo(argv[1], argv[2], &hints, &res);
	if (success != 0)
	{
		std::cout << "ERROR: "<< gai_strerror(success)<<std::endl;
		return success;
	}
	//Init and bind socket
	int sd = socket(res->ai_family, res->ai_socktype, 0);
	bind(sd, (struct sockaddr *)res->ai_addr, res->ai_addrlen);


	//Create a Thread pool
	float porcentaje;
	for (int i = 0; i <= NUM_THREADS; i++)
	{
		porcentaje = 100.0f * ((float)i/(float)NUM_THREADS);
		std::cout << "\rInitializing Threads: " <<porcentaje<<(char)37/*"%""*/;
		pthread_t tid;
		pthread_attr_t attr;
		ServerThread* st = new ServerThread(sd);

		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		pthread_create(&tid, &attr, start_routine, static_cast<void*>(st));
	}
	std::cout << " Done!" << '\n';
	//Main Thread
	while (1) {
		sleep(60);
	}

	std::cout<<"Saliendo...\n";
	freeaddrinfo(res);

	return 0;
};

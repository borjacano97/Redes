#include <ctime>
#include <iostream>

int main(int argc, char** argv)
{
	time_t rawtime;
	tm * timeinfo;

  	time (&rawtime);
  	timeinfo = localtime (&rawtime);
	std::cout<< "Current local time and date: "<< asctime(timeinfo)<< std::endl;
	//std::cin.get();
}

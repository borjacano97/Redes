#include <ctime>
#include <iostream>

int main (int argc, char** argv)
{
	timespec time;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time);
	int a = 0;	
	for (int i = 0; i < 10000; i++) a++;
	std::cout<<"Time spent to increment a "<<a<<" times: "<<time.tv_nsec<<" ns\n";
}

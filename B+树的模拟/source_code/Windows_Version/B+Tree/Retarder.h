#ifndef Retarder_h
#define Retarder_h
#include <Windows.h>

class delay_count {
	long long int count;
public:
	delay_count() {
		count = 0;
	}
	void delay() {
		Sleep(20);
		count++;
	}
	long long int get_count() {
		return count;
	}
};






#endif
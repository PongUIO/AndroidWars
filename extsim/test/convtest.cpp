#include <iostream>
#include <iomanip>
#include <string>
#include <stdint.h>
#include "../util/TypeConv.h"

using namespace std;

void testExec(const std::string &msg, void (*func)())
{
	cout << msg << endl;
	cout << std::string(20,'-') << endl;
	func();
	cout << std::string(20,'-') << "\n\n";
}

void bitfield()
{
	const char *bits = "111100110101xx01";
	
	cout << "Without default (0x0):\n";
	cout << bits << " - " << hex << uppercase <<
		exts::convBitfield<uint64_t>(bits,0) << "\n\n";
	
	cout << "With default (0xF):\n";
	cout << bits << " - " << hex << uppercase <<
		exts::convBitfield<uint64_t>(bits,0xF) << "\n\n";
}

int main(void)
{
	testExec("Testing convBitfield():", bitfield);
	
	return 0;
}

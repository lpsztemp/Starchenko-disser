#include <iostream>
#include <cstdint>
#include <iomanip>
using namespace std;

/*
https://en.cppreference.com/w/c/language/operator_arithmetic
https://en.cppreference.com/w/c/language/for
https://en.cppreference.com/w/c/language/if
*/

uint64_t parity_bit(uint64_t input)
{
	uint64_t result = 0;
	unsigned i;
	for (i = 0; i < 64; i = i + 1)
		result = result ^ ((input >> i) & 1); // result = result ^ ((input << i) & 0x8000000000000000ui64);
	return result;
}

//bitsize(input) < bitsize(poly) - 1
uint64_t crc_1(uint64_t input, uint64_t poly)
{
	uint64_t result = 0;
	// <<, >>, ^, &, |
	return -1;
}


int main(int argc, char** argv)
{
	uint64_t poly = 0x104C11DB7;
	uint64_t input = 12345678901234;
	cout << "Parity bit of " << hex << input << " is " << parity_bit(input) << "\n";
	cout << "CRC of " << hex << input << " with polynomial " << hex << poly << " is " << crc_1(input, poly) << "\n";
	return 0;
}

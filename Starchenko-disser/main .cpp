#include <iostream>
#include <cstdint>
#include <iomanip>
#include <chrono>
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

int parity_bit_vector(const uint8_t* V, unsigned N)
{
	uint8_t result = 0;
	int Re = 0;
	unsigned i;
	for (i = 0; i < N; i = i + 1)
		result = result ^ V[i];
	for (i = 0; i < 8; i = i + 1)
		Re = Re ^ ((result >> i) & 1);
	return Re;
}

int accum_bit_vector(const uint8_t* V, unsigned N)
{
	uint8_t result = 0;
	int accum = 0;
	unsigned i;
	for (i = 0; i < N; i = i + 1)
		result = result + V[i];
	return accum;
}

//bitsize(input) < bitsize(poly) - 1
uint64_t crc_1(uint64_t input, uint64_t poly)
{
	uint64_t result = 0;
	// <<, >>, ^, &, |
	return -1;
}

struct result_and_time
{
	int result;
	std::chrono::milliseconds time;
};

result_and_time get_result_and_time(const uint8_t* V, unsigned N, int (*compute) (const uint8_t* , unsigned))
{
	result_and_time value;
	auto tm0 = std::chrono::system_clock::now();

	value.result = compute(V, N);

	value.time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - tm0);

	return value;
}

int main(int argc, char** argv)
{
	uint64_t poly = 0x104C11DB7;
	uint64_t input = 0x6DC6;
	cout << "Parity bit of " << hex << input << " is " << parity_bit(input) << "\n";
	cout << "Parity bit of " << hex << input << " is " << parity_bit_vector((uint8_t*) &input, sizeof(input)) << "\n";
	cout << "CRC of " << hex << input << " with polynomial " << hex << poly << " is " << crc_1(input, poly) << "\n";

	result_and_time res;
	res = get_result_and_time((uint8_t*) &input, sizeof(input), parity_bit_vector);
	cout << "Result: " << res.result << ", time: " << res.time.count() << "ms.\n";
	res = get_result_and_time((uint8_t*) &input, sizeof(input), accum_bit_vector);
	cout << "Result: " << res.result << ", time: " << res.time.count() << "ms.\n";

	return 0;
}

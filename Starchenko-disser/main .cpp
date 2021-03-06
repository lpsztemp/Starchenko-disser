#include <iostream>
#include <cstdint>
#include <iomanip>
#include <chrono>
#include <immintrin.h>

using namespace std;

/*
https://en.cppreference.com/w/c/language/operator_arithmetic
https://en.cppreference.com/w/c/language/for
https://en.cppreference.com/w/c/language/if
*/

//https://en.cppreference.com/w/cpp/language/integer_literal
//https://en.cppreference.com/w/cpp/language/types

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
		accum = accum + V[i];
	return accum;
}

unsigned bitsize(const unsigned char* val, unsigned byte_size)
{
	unsigned i = byte_size;
	while (i-- > 0)
	{
		if (val[i] != 0)
		{
			unsigned j = 8;
			while (j-- > 0)
			{
				if ((val[i] & (1 << j)) != 0)
					return j + 1 + i * 8;
			}
		}
	}
	return 0;
}

unsigned bitsize(uint64_t val)
{
	return bitsize((const unsigned char*) &val, 8);
}

//bitsize(input) < bitsize(poly) - 1
uint64_t crc_1(uint64_t input, uint64_t poly)
{
	unsigned deg = bitsize(poly) - 1;
	unsigned input_deg = bitsize(input);
	while (input_deg-- >= deg)
	{
		if ((((uint64_t) 1 << input_deg) & input) != 0)
			input = input ^ (poly << (input_deg - deg));
	}
	input = input << deg;
	input_deg = input_deg + deg + 2;
	while (input_deg-- >= deg + 1)
	{
		if (((1ull << input_deg) & input) != 0) //1ull equivalent uint64_t(1)
			input = input ^ (poly << (input_deg - deg));
	}
	return input;
}

/*Implement left_shift? */
uint64_t crc_2(unsigned char* val, size_t val_size, uint64_t poly)
{
	return 0;
}

/*Big-Endian*/

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

unsigned MIN_VECTOR_SIZE = 8;
unsigned MAX_VECTOR_SIZE = 1 << 25;
unsigned STEP_VECTOR_SIZE = (1 << 20);

int main(int argc, char** argv)
{
	uint64_t poly = 0x104C11DB7;
	uint64_t input = 0x1234567812345678ull;
	cout << "Parity bit of " << hex << input << " is " << parity_bit(input) << "\n";
	cout << "Parity bit of " << hex << input << " is " << parity_bit_vector((uint8_t*) &input, sizeof(input)) << "\n";
	cout << "Accum bit of " << hex << input << " is " << accum_bit_vector((uint8_t*) &input, sizeof(input)) << "\n";
	cout << "CRC of " << hex << input << " with polynomial " << hex << poly << " is " << crc_1(input, poly) << "\n";
	uint64_t test_poly = 0xB, test_input = 0B11010011101100;
	cout << "CRC of " << hex << test_input << " with polynomial " << hex << test_poly << " is " << crc_1(test_input, test_poly) << "\n";

	uint8_t* V = (uint8_t*) malloc(MAX_VECTOR_SIZE); //allocate
	for (unsigned i = 0; i < MAX_VECTOR_SIZE; i = i + 1) //initialize
		V[i] = i * 3;

	std::cout << std::dec;

	cout << "Size\tTime\tResult\n";
	for (unsigned i = MIN_VECTOR_SIZE; i < MAX_VECTOR_SIZE; i = i + STEP_VECTOR_SIZE)
	{
		result_and_time res = get_result_and_time(V, i, parity_bit_vector);
		cout << i << '\t' << res.time.count() << '\t' << res.result << '\n';
	}

	cout << "Size\tTime\tResult\n";
	for (unsigned i = MIN_VECTOR_SIZE; i < MAX_VECTOR_SIZE; i = i + STEP_VECTOR_SIZE)
	{
		result_and_time res = get_result_and_time(V, i, accum_bit_vector);
		cout << i << '\t' << res.time.count() << '\t' << res.result << '\n';
	}


	
	result_and_time res;
	res = get_result_and_time((uint8_t*) &input, sizeof(input), parity_bit_vector);
	cout << "Result: " << res.result << ", time: " << res.time.count() << "ms.\n";
	res = get_result_and_time((uint8_t*) &input, sizeof(input), accum_bit_vector);
	cout << "Result: " << res.result << ", time: " << res.time.count() << "ms.\n";
	
	free(V);

	return 0;
}

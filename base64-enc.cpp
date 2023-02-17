#include "nanobench.h"
#include <iostream>
#include <cstdlib>
#include <immintrin.h>


#define SIZE_PACKED 24288
#define SIZE_UNPACKED 32384


/* Input data that will be filled with random numbers */ 
uint8_t data[SIZE_PACKED];


uint8_t lookup[64] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
		              'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
		              'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
		              'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
		              'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
		              'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
		              'w', 'x', 'y', 'z', '0', '1', '2', '3',
		              '4', '5', '6', '7', '8', '9', '+', '/'};
/* Storage where encoded data is being stored */
uint8_t to_print[SIZE_UNPACKED];

/* For benchmarking */
std::size_t operations = 1e4;

/* Lookup table for >> 2 (byte divided by 4 -> each char is repeated 4 times) */
uint8_t lookup_x4[256] = {
'A', 'A', 'A', 'A',
'B', 'B', 'B', 'B',
'C', 'C', 'C', 'C',
'D', 'D', 'D', 'D',
'E', 'E', 'E', 'E',
'F', 'F', 'F', 'F',
'G', 'G', 'G', 'G',
'H', 'H', 'H', 'H',
'I', 'I', 'I', 'I',
'J', 'J', 'J', 'J',
'K', 'K', 'K', 'K',
'L', 'L', 'L', 'L',
'M', 'M', 'M', 'M',
'N', 'N', 'N', 'N',
'O', 'O', 'O', 'O',
'P', 'P', 'P', 'P',
'Q', 'Q', 'Q', 'Q',
'R', 'R', 'R', 'R',
'S', 'S', 'S', 'S',
'T', 'T', 'T', 'T',
'U', 'U', 'U', 'U',
'V', 'V', 'V', 'V',
'W', 'W', 'W', 'W',
'X', 'X', 'X', 'X',
'Y', 'Y', 'Y', 'Y',
'Z', 'Z', 'Z', 'Z',
'a', 'a', 'a', 'a',
'b', 'b', 'b', 'b',
'c', 'c', 'c', 'c',
'd', 'd', 'd', 'd',
'e', 'e', 'e', 'e',
'f', 'f', 'f', 'f',
'g', 'g', 'g', 'g',
'h', 'h', 'h', 'h',
'i', 'i', 'i', 'i',
'j', 'j', 'j', 'j',
'k', 'k', 'k', 'k',
'l', 'l', 'l', 'l',
'm', 'm', 'm', 'm',
'n', 'n', 'n', 'n',
'o', 'o', 'o', 'o',
'p', 'p', 'p', 'p',
'q', 'q', 'q', 'q',
'r', 'r', 'r', 'r',
's', 's', 's', 's',
't', 't', 't', 't',
'u', 'u', 'u', 'u',
'v', 'v', 'v', 'v',
'w', 'w', 'w', 'w',
'x', 'x', 'x', 'x',
'y', 'y', 'y', 'y',
'z', 'z', 'z', 'z',
'0', '0', '0', '0',
'1', '1', '1', '1',
'2', '2', '2', '2',
'3', '3', '3', '3',
'4', '4', '4', '4',
'5', '5', '5', '5',
'6', '6', '6', '6',
'7', '7', '7', '7',
'8', '8', '8', '8',
'9', '9', '9', '9',
'+', '+', '+', '+',
'/', '/', '/', '/'
};

/* Lookup table repeated 4 times (modulo 64) */
uint8_t lookup_m64[256] = {
'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
'w', 'x', 'y', 'z', '0', '1', '2', '3',
'4', '5', '6', '7', '8', '9', '+', '/',

'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
'w', 'x', 'y', 'z', '0', '1', '2', '3',
'4', '5', '6', '7', '8', '9', '+', '/',

'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
'w', 'x', 'y', 'z', '0', '1', '2', '3',
'4', '5', '6', '7', '8', '9', '+', '/',

'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
'w', 'x', 'y', 'z', '0', '1', '2', '3',
'4', '5', '6', '7', '8', '9', '+', '/'
};


int main() {
	time_t t;
	srand((unsigned) time(&t));
	for (std::size_t i = 0; i < SIZE_PACKED; i++) {
		data[i] = rand() % 256;
	}

	/* For SIMD unpacking 2 bytes in each 32-bit interval in the __m256i register */	
	__m256i shuffle_mask = _mm256_set_epi8(
		10 , 11 , 9 , 10 , 7 , 8 , 6 , 7 , 4 , 5 , 3 , 4 , 1 , 2 , 0 , 1 ,
		14 , 15 , 13 , 14 , 11 , 12 , 10 , 11 , 8 , 9 , 7 , 8 , 5 , 6 , 4 , 5
		);

	__m256i and_mask = _mm256_set1_epi32(0xfc003f0);
	__m256i mul_mask = _mm256_set1_epi32(0x00100040);
	/* ========================================================================= */

	/* Benchmark model */
	auto bench = ankerl::nanobench::Bench().minEpochIterations(operations).relative(true);
	bench.run("avx2 base64 encode", [&] {
		for (int i = 0; i < SIZE_PACKED; i += 24) {
			__m256i input = _mm256_loadu_si256((__m256i*)(&data[i]-4));
		
			__m256i in = _mm256_shuffle_epi8(input, shuffle_mask);
			__m256i t0 = _mm256_and_si256(in, and_mask);
			__m256i t1 = _mm256_mulhi_epu16(t0, mul_mask);
			
			to_print[i+0] = lookup_x4[data[0]];
			to_print[i+1] = lookup[((uint8_t*)&t1)[1]];
			to_print[i+2] = lookup[((uint8_t*)&t1)[3]];
			to_print[i+3] = lookup_m64[data[3]];
			to_print[i+4] = lookup_x4[data[4]];
			to_print[i+5] = lookup[((uint8_t*)&t1)[5]];
			to_print[i+6] = lookup[((uint8_t*)&t1)[7]];
			to_print[i+7] = lookup_m64[data[7]];
			
			to_print[i+8] = lookup_x4[data[8]];
			to_print[i+9] = lookup[((uint8_t*)&t1)[9]];
			to_print[i+10] = lookup[((uint8_t*)&t1)[11]];
			to_print[i+11] = lookup_m64[data[11]];
			to_print[i+12] = lookup_x4[data[12]];
			to_print[i+13] = lookup[((uint8_t*)&t1)[13]];
			to_print[i+14] = lookup[((uint8_t*)&t1)[15]];
			to_print[i+15] = lookup_m64[data[15]];
			
			to_print[i+16] = lookup_x4[data[16]];
			to_print[i+17] = lookup[((uint8_t*)&t1)[17]];
			to_print[i+18] = lookup[((uint8_t*)&t1)[19]];
			to_print[i+19] = lookup_m64[data[19]];
			to_print[i+20] = lookup_x4[data[20]];
			to_print[i+21] = lookup[((uint8_t*)&t1)[21]];
			to_print[i+22] = lookup[((uint8_t*)&t1)[23]];
			to_print[i+23] = lookup_m64[data[23]];
			
			to_print[i+24] = lookup_x4[data[24]];
			to_print[i+25] = lookup[((uint8_t*)&t1)[25]];
			to_print[i+26] = lookup[((uint8_t*)&t1)[27]];
			to_print[i+27] = lookup_m64[data[27]];
			to_print[i+28] = lookup_x4[data[28]];
			to_print[i+29] = lookup[((uint8_t*)&t1)[29]];
			to_print[i+30] = lookup[((uint8_t*)&t1)[31]];
			to_print[i+31] = lookup_m64[data[31]];
		}
	});
	
	bench.run("google chrome base64 encode", [&] {
	
		for (int j = 0; j < SIZE_PACKED; j+=3) {
			to_print[j] = lookup_x4[data[j+0]];
			to_print[j+1] = lookup[((data[j+0] * 16) % 64) | (data[j+1] / 16)];
			to_print[j+2] = lookup[((data[j+1] * 4) % 64) | (data[j+2] % 64)];
			to_print[j+3] = lookup_m64[data[j+2]];
		}
	});
	
	return 0;
}

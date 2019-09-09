#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#include <stdlib.h>
#include <time.h>

#define StartBlock()	(code_ptr = dst++, code = 1)
#define FinishBlock()	(*code_ptr = code)

size_t StuffData(uint_fast8_t *ptr, const size_t length, uint_fast8_t *dst)
{
	const uint_fast8_t *start = dst, *end = ptr + length;
	uint_fast8_t code, *code_ptr; // Where to insert the leading count
	StartBlock();
	
	while (ptr < end) {//loop over data
		if (code != 0xFF) {
			uint_fast8_t c = *ptr++;
			if (c != 0) {
				*dst++ = c;
				code++;
				continue;
			}
		}
		FinishBlock();
		StartBlock();
	}
	FinishBlock();
	return dst - start;
}

size_t UnStuffData(uint_fast8_t *ptr, size_t length, uint_fast8_t *dst)
{
	const uint_fast8_t *start = dst, *end = ptr + length;
	uint_fast8_t code = 0xFF, copy = 0;

	for (; ptr < end; copy--) {
		if (copy != 0) {
			*dst++ = *ptr++;
		} else {
			if (code != 0xFF)
				*dst++ = 0;
			copy = code = *ptr++;
			if (code == 0)
				break;
		}
	}
	return dst - start;
}

int main()
{
	const int frame_length = 254; 
	char s[frame_length];
	uint_fast8_t s_byte[frame_length];
	uint_fast8_t out[frame_length+2];
	uint_fast8_t test[frame_length];
	
	//Generate
	
	int i;
	srand(time(NULL));
	for (i=0;i<frame_length;i++){
		//s[i]=(rand()%26)+65;//letter space
		s[i]=rand() % 256;//arbitrary
		//s[i] = 0;//reserved byte mine field
	}
	//s[0] = 0xff;//testing altered first bit
	
	//printf("string: \t%s\n",s);
	printf("Random Byte Frame (%d Bytes).\n",frame_length);
	
	//to byte array.
	for (i=0;i<frame_length;i++)
		s_byte[i]= (uint8_t) s[i];
	
	//COBS
	double time_spent = 0.0;
	clock_t begin = clock();
	size_t length = StuffData(s_byte, frame_length, out);
	clock_t end = clock();
	
	time_spent = (double)(end - begin)*1000000 / CLOCKS_PER_SEC;

	printf("Time elpased is %d microseconds.\n", (int)time_spent);
	printf("Total Size: %lu Bytes (+%lu).",length,length-frame_length);
	printf("\n=================================\n");
	printf("Data before substitution\n");
	printf("=================================\n");
	
	size_t length2 = UnStuffData(out, frame_length+1, test);
	for (int i = 0; i < frame_length; i++){
		printf("0x%02x|", s_byte[i]);
		if (i % 8 == 7)
			printf("\n");
	}
	//printf("%x",length);
	printf("\n=================================\n");
	printf("Data after substitution\n");
	printf("=================================\n");
	for (int i = 0; i < length; i++){
		printf("0x%02x|", out[i]);
		if (i % 8 == 7)
			printf("\n");
	}
	printf("\n=================================\n");
	printf("Equality check: Data == Inverse_Cobs(Cobs(Data))\n");
	printf("=================================\n");
	for (int i = 0; i < length2; i++){
		printf("%s ", (s_byte[i] == test[i]) ? "true" : "false");//stuffed then unstuffed check
		if (i % 8 == 7)
			printf("\n");
	}
	return 0;
}

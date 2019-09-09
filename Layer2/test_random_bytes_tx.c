#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>


uint_fast32_t crc32_custom(uint_fast32_t crc, const char *buf, size_t len)
{
	uint_fast8_t octet;
	const char *p, *q;

	static uint_fast32_t table[256] = 
	{
		0x00000000,0x9960034c,0xaba005d4,0x32c00698,
		0xce2008e4,0x57400ba8,0x65800d30,0xfce00e7c,
		0x05201284,0x9c4011c8,0xae801750,0x37e0141c,
		0xcb001a60,0x5260192c,0x60a01fb4,0xf9c01cf8,
		0x0a402508,0x93202644,0xa1e020dc,0x38802390,
		0xc4602dec,0x5d002ea0,0x6fc02838,0xf6a02b74,
		0x0f60378c,0x960034c0,0xa4c03258,0x3da03114,
		0xc1403f68,0x58203c24,0x6ae03abc,0xf38039f0,
		0x14804a10,0x8de0495c,0xbf204fc4,0x26404c88,
		0xdaa042f4,0x43c041b8,0x71004720,0xe860446c,
		0x11a05894,0x88c05bd8,0xba005d40,0x23605e0c,
		0xdf805070,0x46e0533c,0x742055a4,0xed4056e8,
		0x1ec06f18,0x87a06c54,0xb5606acc,0x2c006980,
		0xd0e067fc,0x498064b0,0x7b406228,0xe2206164,
		0x1be07d9c,0x82807ed0,0xb0407848,0x29207b04,
		0xd5c07578,0x4ca07634,0x7e6070ac,0xe70073e0,
		0x29009420,0xb060976c,0x82a091f4,0x1bc092b8,
		0xe7209cc4,0x7e409f88,0x4c809910,0xd5e09a5c,
		0x2c2086a4,0xb54085e8,0x87808370,0x1ee0803c,
		0xe2008e40,0x7b608d0c,0x49a08b94,0xd0c088d8,
		0x2340b128,0xba20b264,0x88e0b4fc,0x1180b7b0,
		0xed60b9cc,0x7400ba80,0x46c0bc18,0xdfa0bf54,
		0x2660a3ac,0xbf00a0e0,0x8dc0a678,0x14a0a534,
		0xe840ab48,0x7120a804,0x43e0ae9c,0xda80add0,
		0x3d80de30,0xa4e0dd7c,0x9620dbe4,0x0f40d8a8,
		0xf3a0d6d4,0x6ac0d598,0x5800d300,0xc160d04c,
		0x38a0ccb4,0xa1c0cff8,0x9300c960,0x0a60ca2c,
		0xf680c450,0x6fe0c71c,0x5d20c184,0xc440c2c8,
		0x37c0fb38,0xaea0f874,0x9c60feec,0x0500fda0,
		0xf9e0f3dc,0x6080f090,0x5240f608,0xcb20f544,
		0x32e0e9bc,0xab80eaf0,0x9940ec68,0x0020ef24,
		0xfcc0e158,0x65a0e214,0x5760e48c,0xce00e7c0,
		0x52012840,0xcb612b0c,0xf9a12d94,0x60c12ed8,
		0x9c2120a4,0x054123e8,0x37812570,0xaee1263c,
		0x57213ac4,0xce413988,0xfc813f10,0x65e13c5c,
		0x99013220,0x0061316c,0x32a137f4,0xabc134b8,
		0x58410d48,0xc1210e04,0xf3e1089c,0x6a810bd0,
		0x966105ac,0x0f0106e0,0x3dc10078,0xa4a10334,
		0x5d611fcc,0xc4011c80,0xf6c11a18,0x6fa11954,
		0x93411728,0x0a211464,0x38e112fc,0xa18111b0,
		0x46816250,0xdfe1611c,0xed216784,0x744164c8,
		0x88a16ab4,0x11c169f8,0x23016f60,0xba616c2c,
		0x43a170d4,0xdac17398,0xe8017500,0x7161764c,
		0x8d817830,0x14e17b7c,0x26217de4,0xbf417ea8,
		0x4cc14758,0xd5a14414,0xe761428c,0x7e0141c0,
		0x82e14fbc,0x1b814cf0,0x29414a68,0xb0214924,
		0x49e155dc,0xd0815690,0xe2415008,0x7b215344,
		0x87c15d38,0x1ea15e74,0x2c6158ec,0xb5015ba0,
		0x7b01bc60,0xe261bf2c,0xd0a1b9b4,0x49c1baf8,
		0xb521b484,0x2c41b7c8,0x1e81b150,0x87e1b21c,
		0x7e21aee4,0xe741ada8,0xd581ab30,0x4ce1a87c,
		0xb001a600,0x2961a54c,0x1ba1a3d4,0x82c1a098,
		0x71419968,0xe8219a24,0xdae19cbc,0x43819ff0,
		0xbf61918c,0x260192c0,0x14c19458,0x8da19714,
		0x74618bec,0xed0188a0,0xdfc18e38,0x46a18d74,
		0xba418308,0x23218044,0x11e186dc,0x88818590,
		0x6f81f670,0xf6e1f53c,0xc421f3a4,0x5d41f0e8,
		0xa1a1fe94,0x38c1fdd8,0x0a01fb40,0x9361f80c,
		0x6aa1e4f4,0xf3c1e7b8,0xc101e120,0x5861e26c,
		0xa481ec10,0x3de1ef5c,0x0f21e9c4,0x9641ea88,
		0x65c1d378,0xfca1d034,0xce61d6ac,0x5701d5e0,
		0xabe1db9c,0x3281d8d0,0x0041de48,0x9921dd04,
		0x60e1c1fc,0xf981c2b0,0xcb41c428,0x5221c764,
		0xaec1c918,0x37a1ca54,0x0561cccc,0x9c01cf80,
	};
	crc = ~crc; //initial crc
	q = buf + len;
	for (p = buf; p < q; p++) {
		octet = *p;  /* Cast to unsigned octet. */
		crc = (crc >> 8) ^ table[(crc & 0xff) ^ octet];
	}
	return ~crc;//final crc
}

#define StartBlock()	(code_ptr = dst++, code = 1)
#define FinishBlock()	(*code_ptr = code)

size_t StuffData(uint_fast8_t *ptr, size_t length, uint_fast8_t *dst)
{
	const uint8_t *start = dst, *end = ptr + length;
	uint_fast8_t code, *code_ptr; // Where to insert the leading count
	StartBlock();
	
	while (ptr < end) {//loop over data
		if (code != 0xFF) {
			uint8_t c = *ptr++;
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
	const int packet_length = 250; //size with header, size without is 245
	char s[packet_length];
	
	uint_fast32_t crc_digest;
	int i;
	srand(time(0));
	
	printf("=================================\n");
	printf("Create and Frame Packet\n");
	printf("=================================\n");
	
	//Generate
	for (i=0;i<packet_length;i++){
		s[i]=rand() % 256;//arbitrary
	}
	
	printf("Random Payload (245 Bytes).\nAdd Random Header\n");
	printf("New Size: 250 Bytes (+5).\n");
	
	double time_spent = 0.0;
	clock_t begin = clock();
	crc_digest = crc32_custom(0, s, 250);
	//printf("\n%x/n",strlen(s));
	clock_t end = clock();
	
	//CRC-32
	printf("=================================\n");
	printf("CRC-32: 0x%x\n", crc_digest);
	
	time_spent = (double)(end - begin)*1000000 / CLOCKS_PER_SEC;

	printf("Time for step is %d microseconds.\n", (int)time_spent);
	printf("New Size: 254 Bytes (+4).\n");
	
	//COBS
	printf("=================================\n");
	printf("COBS Substitution of 0x0\n");
	
	uint_fast8_t s_byte[254];
	uint_fast8_t out[255];
	
	//s_byte[0] = 0x0;//add start of frame
	
	for (i=0;i<250;i++)//payload
		s_byte[i]= (uint8_t) s[i];

	s_byte[250] = (crc_digest >> 24) & 0xFF;//add crc-32 as 4 8-bit entries
	s_byte[251] = (crc_digest >> 16) & 0xFF;
	s_byte[252] = (crc_digest >> 8) & 0xFF;
	s_byte[253] = crc_digest & 0xFF;
	
	begin = clock();
	StuffData(s_byte, 254, out);
	end = clock();
	
	time_spent = (double)(end - begin)*1000000 / CLOCKS_PER_SEC;

	printf("Time for step is %d microseconds.\n", (int)time_spent);
	printf("Total Size: 256 Bytes (+2).\n");
	
	printf("\n=================================\n");
	printf("Deframe and Check Packet\n");
	printf("=================================\n");
	printf("Undo COBS Substitution of 0x0\n");
	
	uint_fast8_t s_byte_hat[254];
	
	begin = clock();
	UnStuffData(out, 255, s_byte_hat);
	end = clock();
	
	time_spent = (double)(end - begin)*1000000 / CLOCKS_PER_SEC;
	
	uint_fast32_t crc_rx;
	uint_fast32_t crc_digest_hat;
	uint_fast8_t payload[250];

	for(i = 0;i < 250; i++)
		payload[i] = s_byte_hat[i];
	
	begin = clock();
	crc_rx = (s_byte_hat[250] << 24) + (s_byte_hat[251] << 16) + (s_byte_hat[252] << 8) + s_byte_hat[253];
	crc_digest_hat = crc32_custom(0, payload, 250);
	end = clock();
	
	printf("Time for step is %d microseconds.\n", (int)time_spent);
	printf("RX CRC: 0x%x\n", crc_rx);
	printf("=================================\n");
	printf("Check CRC of RX frame\n");
	printf("Computed CRC: 0x%x\n",crc_digest_hat);
	printf("Matches?: %s\n", (int) crc_rx == (int) crc_digest_hat ? "True":"False");
	time_spent = (double)(end - begin)*1000000 / CLOCKS_PER_SEC;
	printf("Time for step is %d microseconds.\n", (int)time_spent);
	printf("=================================\n");
	printf("Payload + Header (250 Bytes).\n");
	
	/*
	printf("DEBUG PAYLOAD\n");
	for (int i = 0; i < 250; i++){
		printf("0x%02x|", (uint_fast8_t) s[i]);
		if (i % 8 == 7)
			printf("\n");
	}
		printf("=================================\n");

	for (int i = 0; i < 250; i++){
		printf("0x%02x|", payload[i]);
		if (i % 8 == 7)
			printf("\n");
	}
	*/
	
	return 0;
}
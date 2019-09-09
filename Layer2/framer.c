#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

#include "framer.h"

size_t create_segment(uint_fast8_t ID, uint_fast16_t seq_num, uint_fast16_t ack, uint_fast8_t *ptr, const size_t length, uint_fast8_t *dst){
	uint_fast8_t raw_frame[length+5]; //Header and payload
	uint_fast8_t segment[length+9]; //Header, payload and crc
	uint_fast8_t out[length+11]; //COBS substitution and trailing 0x0
	
	//assign header
	raw_frame[0] = ID; //ID
	raw_frame[1] = (seq_num >> 8) & 0xff; //seq_num first byte
	raw_frame[2] = seq_num & 0xff; //seq_num second byte
	raw_frame[3] = (ack >> 8) & 0xff; //ack first byte
	raw_frame[4] = ack & 0xff; //ack second byte
	
	int i;
	for (i=5;i<length+5;i++)//add payload to frame
		raw_frame[i] = *ptr++;
	
	uint_fast32_t crc_digest = crc32_custom(0, raw_frame, length+5);
	
	segment[length+5] = (crc_digest >> 24) & 0xFF;//add crc-32 as 4 entries in the byte array
	segment[length+6] = (crc_digest >> 16) & 0xFF;
	segment[length+7] = (crc_digest >> 8) & 0xFF;
	segment[length+8] = crc_digest & 0xFF;
	
	for (i=0;i<length+5;i++)
		segment[i] = raw_frame[i];
	
	StuffData(segment, length+9, out);
	
	out[length+10] = 0x0; //explicitly add trailing 0
	
	for (i=0;i<length+11;i++)//payload
		*dst++ = out[i];
	return (size_t)(length+11);
}

size_t split_and_frame(uint16_t ack, uint_fast8_t *ptr, const size_t length, const size_t target_payload_size, uint_fast8_t *dst){
	const int target_packet_size = target_payload_size + 11;
	uint_fast8_t data[length];
	uint_fast8_t seg[target_payload_size+11];
	uint16_t seq_num = 0;
	int total = length;
	int i;
	
	*dst++ = 0x0;//add leading 0x0 (start of first frame)
	
	do{
		if(seq_num+target_payload_size<=length){
			for (i=0;i<target_packet_size;i++)//pack bytes to target_packet_size
				data[i] = *ptr++;
			total+=create_segment(0x0, seq_num, ack, data, target_packet_size, seg);//placeholder for ID: 0x0
			for (i=0;i<target_packet_size+11;i++)//output segment
				*dst = seg[i];
			seq_num+=target_packet_size;
		}else{
			int end = length % seq_num;
			uint_fast8_t data_last[end];
			uint_fast8_t seg_last[end+11];
			for (i=0;i<end;i++)//pack last bytes
				data_last[i] = *ptr++;
			total+=create_segment(0x0,seq_num,ack,data_last, end, seg_last);
			seq_num+=end;
		}
	}while(seq_num!=length);
	return (size_t)total;
}

int main(){
	printf("test");
}


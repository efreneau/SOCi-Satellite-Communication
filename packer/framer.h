//framing and deframing functions for the communication system intended to be used on the Soci cubesat.
//https://github.com/efreneau/SOCi-Satellite-Communication

#ifndef _framer_h
#define _framer_h

#include "crc32_custom.h"
#include "COBS.h"

size_t create_segment(uint_fast8_t ID, uint_fast16_t seq_num, uint_fast16_t ack, uint_fast8_t *ptr, const size_t length, uint_fast8_t *dst);
//ID is to keep track of what is being transfered (might be removed)
//seq_num is the byte # corresponding to the first byte in the current payload (tx side)
//ack is the byte # referring to the last error free byte (rx side)
//*ptr points to the payload array
//*dst points to the output array

size_t split_and_frame(uint16_t ack, uint_fast8_t *ptr, const size_t length, const size_t target_payload_size, uint_fast8_t *dst);
//ack is the byte # referring to the last error free byte (rx side)
//*ptr points to the payload array
//length refers to the total length in bytes of the data object
//*dst points to the output array

#endif

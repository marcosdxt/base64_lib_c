#ifndef _BASE64_H_
#define _BASE64_H_



void base64_encode(uint8_t *bin_vector,uint16_t in_size,uint8_t *base64_vector,uint16_t *out_size);
void base64_decode(uint8_t *base64_vector,uint16_t in_size,uint8_t *bin_vector,uint16_t *out_size);






#endif//_BASE64_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const uint8_t  base64_encoding_table[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/'
};

// Joga fora um bocado de memória.... Eu sei...Mas flash não é o problema...
static const uint8_t base_decoding_table[]={
    ['A']=0,  ['B']=1,  ['C']=2,  ['D']=3,  ['E']=4,  ['F']=5,  ['G']=6,  ['H']=7,
    ['I']=8,  ['J']=9,  ['K']=10, ['L']=11, ['M']=12, ['N']=13, ['O']=14, ['P']=15,
    ['Q']=16, ['R']=17, ['S']=18, ['T']=19, ['U']=20, ['V']=21, ['W']=22, ['X']=23,
    ['Y']=24, ['Z']=25, ['a']=26, ['b']=27, ['c']=28, ['d']=29, ['e']=30, ['f']=31,
    ['g']=32, ['h']=33, ['i']=34, ['j']=35, ['k']=36, ['l']=37, ['m']=38, ['n']=39,
    ['o']=40, ['p']=41, ['q']=42, ['r']=43, ['s']=44, ['t']=45, ['u']=46, ['v']=47,
    ['w']=48, ['x']=49, ['y']=50, ['z']=51, ['0']=52, ['1']=53, ['2']=54, ['3']=55,
    ['4']=56, ['5']=57, ['6']=58, ['7']=59, ['8']=60, ['9']=61, ['+']=62, ['/']=63
};

void base64_encode(uint8_t *bin_vector,uint16_t in_size,uint8_t *base64_vector,uint16_t *out_size){
  uint32_t tmp;
  uint16_t index=0;
  uint16_t out=0;

  while(in_size){

      if(in_size>=3) {
          in_size-=3;

          tmp = bin_vector[index]<<16 | bin_vector[index+1]<<8 | bin_vector[index+2];

          base64_vector[out++] = base64_encoding_table[(tmp>>18)&0x3F];
          base64_vector[out++] = base64_encoding_table[(tmp>>12)&0x3F];
          base64_vector[out++] = base64_encoding_table[(tmp>>6)&0x3F];
          base64_vector[out++] = base64_encoding_table[tmp&0x3F];

          index+=3;
      }
      else{
          switch(in_size){
              case 1:
                  tmp = bin_vector[index]<<16;
                  base64_vector[out++] = base64_encoding_table[(tmp>>18)&0x3F];
                  base64_vector[out++] = base64_encoding_table[(tmp>>12)&0x3F];
                  base64_vector[out++] = '=';
                  base64_vector[out++] = '=';
                  break;
              case 2:
                  tmp = bin_vector[index+1]<<8 | bin_vector[index]<<16;
                  base64_vector[out++] = base64_encoding_table[(tmp>>18)&0x3F];
                  base64_vector[out++] = base64_encoding_table[(tmp>>12)&0x3F];
                  base64_vector[out++] = base64_encoding_table[(tmp>>6)&0x3F];
                  base64_vector[out++] = '=';
                  break;
          }
          in_size = 0;
      }
  }

  *out_size = out;
}

uint8_t base64_char_to_bin(uint8_t base64_char) {

    if( (base64_char>='A' && base64_char<='Z') ||
        (base64_char>='a' && base64_char<='z') ||
        (base64_char>='0' && base64_char<='9') ||
        (base64_char=='+') ||
        (base64_char=='/') ){

        return base_decoding_table[base64_char];
    }

    return 0;
}

void base64_decode(uint8_t *base64_vector,uint16_t in_size,uint8_t *bin_vector,uint16_t *out_size){
    uint32_t tmp;
    uint16_t out=0;
    uint16_t index=0;
    uint8_t digitos[4];

    while(in_size){

        if(in_size>=4){

            digitos[0] = base64_char_to_bin(base64_vector[index]);
            digitos[1] = base64_char_to_bin(base64_vector[index+1]);
            digitos[2] = base64_char_to_bin(base64_vector[index+2]);
            digitos[3] = base64_char_to_bin(base64_vector[index+3]);

            tmp = digitos[0]<<18 | digitos[1]<<12 | digitos[2]<<6 | digitos[3];

            bin_vector[out++] = tmp>>16;
            bin_vector[out++] = tmp>>8;
            bin_vector[out++] = tmp;

            index+=4;
            in_size-=4;
        } else {

            switch(in_size){
                case 3:
                    digitos[2] = base64_char_to_bin(base64_vector[index+2]);
                case 2:
                    digitos[1] = base64_char_to_bin(base64_vector[index+1]);
                case 1:
                    digitos[0] = base64_char_to_bin(base64_vector[index]);
            }

            switch(in_size){
                case 3:
                    tmp = digitos[0]<<18 | digitos[1]<<12 | digitos[2]<<6;
                    bin_vector[out++] = tmp>>16;
                    bin_vector[out++] = tmp>>8;
                    bin_vector[out++] = tmp;
                    break;
                case 2:
                    tmp = digitos[0]<<12 | digitos[1]<<6;
                    bin_vector[out++] = tmp>>8;
                    bin_vector[out++] = tmp;
                    break;
                case 1:
                    bin_vector[out++] = digitos[0]<<6;
                    break;
            }


            in_size = 0;
        }
    }

    *out_size = out;
}


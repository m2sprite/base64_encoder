#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "log.h"

typedef int32_t s32;
typedef s32 b32;
typedef uint32_t u32;
typedef uint8_t u8;

#define SHIFT_ARGS( ArgCount , Args ) (assert((ArgCount)>0), (ArgCount)--, *(Args)++)

void print_binary_u32(u32 value)
{
  for(s32 i = 31; i >=0 ; i--)
  {
    printf("%d", (value >> i) & 1);
  }
  printf("\n");
}


size_t cstr_length_without_null_delim( char *c_str )
{
  char *current_char = c_str;
  while(*current_char)
  {
    ++current_char;
  }
  return(current_char - c_str);
}

int main( int ArgCount, char **Args )
{
  SHIFT_ARGS( ArgCount, Args );
  if( ArgCount > 0 )
  {
    char *encode_me =  *Args;
    size_t original_length = cstr_length_without_null_delim( encode_me );
    if ( original_length != 0 )
    {
      // Hel|lo |Bas|e64|Hel|lo |Bas|e64|Hel|lo |Bas|e64|\n"
      // We add 2 because its the highest remainder
      // for example here we have 12 groups of 3 bytes and 1 byte as a remainder
      // adding 2 accounts for it because its the highest possible remainder
      // so any time there is a non 0 remainder it rounds it up
      // here we'd add 2 to our 37 chars making the calcuation into 39/3 = 13 so we'd have 13 groups for 4 bytes which is what we need
      // if we had 38 the remainder would be 2 so we'd have 2 remaining chars the calculation would be (38+2)/3 40/3 = 13 so it would still stay the same amount of groups we need
      // tldr add the highest possible remainder to achieve this
      // this is the trad way of doing it
      const char *ascii_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
      size_t encoding_buffer_length_bytes = ((original_length+2)/3)*4;
      char *encoded = malloc( sizeof(char) * (encoding_buffer_length_bytes+1) );
      encoded[encoding_buffer_length_bytes] = '\0';

      size_t remainder = original_length%3;
      size_t end_of_main_loop = original_length - remainder;
      u32 packer;
      size_t encoded_buffer_idx = 0;
      for(size_t encode_me_i = 0;
            encode_me_i < end_of_main_loop;
            encode_me_i+=3)
      {
        packer = 0;
        packer = (packer | ( (encode_me[encode_me_i]  ) << 16 ));
        packer = (packer | ( (encode_me[encode_me_i+1]) << 8  ));
        packer = (packer |    encode_me[encode_me_i+2]        );

        u8 first_b64_encoded_idx =  (packer & 0b00000000111111000000000000000000) >> 18;
        u8 second_b64_encoded_idx = (packer & 0b00000000000000111111000000000000) >> 12;
        u8 third_b64_encoded_idx =  (packer & 0b00000000000000000000111111000000) >> 6;
        u8 fourth_b64_encoded_idx = (packer & 0b00000000000000000000000000111111);

        encoded[encoded_buffer_idx]   = ascii_table[first_b64_encoded_idx];
        encoded[encoded_buffer_idx+1] = ascii_table[second_b64_encoded_idx];
        encoded[encoded_buffer_idx+2] = ascii_table[third_b64_encoded_idx];
        encoded[encoded_buffer_idx+3] = ascii_table[fourth_b64_encoded_idx];
        encoded_buffer_idx+=4;
      }

      if(remainder == 1)
      {
        packer = 0;
        packer = (packer | ( (encode_me[original_length-1]  ) << 16 ));
        u8 first_b64_encoded_idx =  (packer & 0b00000000111111000000000000000000) >> 18;
        u8 second_b64_encoded_idx = (packer & 0b00000000000000111111000000000000) >> 12;
        encoded[encoded_buffer_idx]   = ascii_table[first_b64_encoded_idx];
        encoded[encoded_buffer_idx+1] = ascii_table[second_b64_encoded_idx];
        encoded[encoded_buffer_idx+2] = '=';
        encoded[encoded_buffer_idx+3] = '=';
      }
      else if(remainder == 2)
      {
        packer = 0;
        packer = (packer | ( (encode_me[original_length-2]  ) << 16 ));
        packer = (packer | ( (encode_me[original_length-1]  ) << 12 ));
        u8 first_b64_encoded_idx =  (packer & 0b00000000111111000000000000000000) >> 18;
        u8 second_b64_encoded_idx = (packer & 0b00000000000000111111000000000000) >> 12;
        u8 third_b64_encoded_idx =  (packer & 0b00000000000000000000111111000000) >> 6;
        encoded[encoded_buffer_idx]   = ascii_table[first_b64_encoded_idx];
        encoded[encoded_buffer_idx+1] = ascii_table[second_b64_encoded_idx];
        encoded[encoded_buffer_idx+2] = ascii_table[third_b64_encoded_idx];
        encoded[encoded_buffer_idx+3] = '=';
      }
      printf("%s\n", encoded);
    }
    else
    {
      Log( ERROR, "Please provide a non zero cstr to encode" );
    }
  }
  else
  {
    Log( ERROR, "Please provide a cstr to encode" );
  }
  return(0);
}

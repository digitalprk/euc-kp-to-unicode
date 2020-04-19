#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "euckpmap.h"

int main(int argc, char *argv[])

{
	FILE *input_file;
	FILE *output_file;
	long length;
	unsigned char *file_content;
	unsigned char *converted_content;
	uint16_t char_index = 0; 
	uint16_t converted_char_value = 0;
	int j = 2;
	
	if (argc != 3) {
		printf("Please provide an input and an output file");
	}
	
	input_file = fopen(argv[1], "r");
	if (input_file == NULL) {
		printf("Error opening input file %s\n", argv[1]);
		return 1;
	}
	
	fseek(input_file, 0L, SEEK_END);
	length = ftell(input_file);
	file_content = malloc (length);
	converted_content = malloc (length*2);
	memset(file_content, 0, length);
	memset(converted_content, 0, length*2);
	fseek(input_file, 0L, SEEK_SET);
	
	if (file_content)
	{
		fread(file_content, 1, length, input_file);
	}
	fclose(input_file);
	
	converted_content[0] = 0xFE;
	converted_content[1] = 0xFF; //Byte order mark
	
	for (int i=0; i < length; i++) {
		if (file_content[i] < 0x80) {
			converted_content[j] = 0;
			converted_content[j+1] = file_content[i];
		}
		else {
			char_index = (file_content[i] << CHAR_BIT) | (file_content[i+1]);
			
			if (char_index < 0x8141) {
				printf("Invalid character found at index %d", i);
				return 1;
			}
			
			char_index = char_index - 0x8141;
			converted_char_value = eucKpMap[char_index];
			if (converted_char_value == 0xFFFF) {
				printf("Invalid character found at index %d", i);
				return 1;
			}
			
			converted_content[j] = (converted_char_value >> 8) & 0xFF;
			converted_content[j+1] = converted_char_value & 0xFF;
			i++;
		}
		j+=2;
	}
	output_file = fopen(argv[2], "w");
	fwrite(converted_content, 1, j, output_file);
	fclose(output_file);
}
			
			
	
	

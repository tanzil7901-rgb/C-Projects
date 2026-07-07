#include <stdio.h>
#include <stdlib.h>
char buffer[17];
FILE *file_ptr;

void read(FILE *file_ptr){
	rewind(file_ptr);
  int c;
	int i = 0;
  printf("0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F  |     Content     \n");
	printf("------------------------------------------------------------------\n");

  while((c = getc(file_ptr)) != EOF){
		if (c >= 32 && c <= 126) {
    	buffer[i] = c;
		} 
		else {
    	buffer[i] = '#'; 
		}
		printf("%02X ", c);
		i++;
		if(i == 16){
		  buffer[16] = '\0';
			printf("| %s\n", buffer);
			i = 0;
		}
	}

	if(c == EOF && i < 16 && i > 0){
		int spaces = 16 - i;
		buffer[i] = '\0';
		for(int j = 0; j < spaces; j++){
			printf("   ");
		}
		printf("| ");
		for(int j = 0; j < i; j++){
			printf("%c", buffer[j]);
		}
	}
}

void clear_input_buffer(){
  int garbage;
  while ((garbage = getchar()) != '\n' && garbage != EOF);
}

void write(FILE *file_ptr){
	unsigned int address;
	unsigned int value;

	fseek(file_ptr, 0, SEEK_END);
	long file_size = ftell(file_ptr);
	printf("\n");

	printf("Enter hex-address to edit > ");
	if(scanf(" %02X", &address) != 1){
		printf("Error: invalid hexadecimal address\n");
		clear_input_buffer();
		return;
	}
	else if(address >= file_size){
		printf("Error: address is out of bounds\n");
		clear_input_buffer();
		return;
	}

	printf("Enter new hex value >");
	if(scanf(" %02X", &value) != 1){
		printf("Error: invalid hexadecimal value\n");
		clear_input_buffer();
		return;
	}
	else if(value > 0xFF){
		printf("Error: hex value is too big\n");
		clear_input_buffer();
		return;
	}

	fseek(file_ptr, address, SEEK_SET);
	putc(value, file_ptr);
	fflush(file_ptr);
	system("clear");
}

int main(int argc, char *argv[]){
	if(argc != 2){
		return 1;
	}
	file_ptr = fopen(argv[1], "r+b");
	while(1){
		read(file_ptr);
		write(file_ptr);
	}
	fclose(file_ptr);
	return 0;
} 
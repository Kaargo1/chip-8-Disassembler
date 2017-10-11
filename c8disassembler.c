#include <stdio.h>
#include <stdlib.h>


void DisassembleCH8(unsigned char* buffer, int pc){
	unsigned char* code = &buffer[pc];
	//Shifts opcode 4 bits to the right so it only gets the first nibble.
	//(In chip8 only the first nibble is important to figure out the instruction)
	unsigned char firstNibble = (code[0] >> 4);
	printf("%04x %02x %02x ", pc, code[0], code[1]);
	int opbytes = 1;
	unsigned char secondNib = code[0] & 0x0f;
	unsigned char thirdNib = code[1] >> 4;
	unsigned char lastNib = code[1] & 0x0f;
	switch(firstNibble){
		case 0x00:
			if(code[0] == 0x00 && code[1] == 0xE0){
				printf("CLS");
			}else if(code[0] == 0x00 && code[1] == 0xee){
				printf("RET");
			}else{
				printf("SYS %01x%02x", secondNib, code[1]);
			}
			break;
		case 0x01:
			{
			printf("JMP %01x%02x", secondNib, code[1]);
			}
			break;
		case 0x02:
			{
			printf("JSR %01x%02x", secondNib, code[1]);
			}
			break;
		case 0x03:
			printf("SE V%01x %02x", secondNib, code[1]);
			break;
		case 0x04:
			printf("SNE V%01x %02x", secondNib, code[1]);
			break;
		case 0x05:
			printf("SE V%01x V%01x", secondNib, code[1]);
			break;
		case 0x06:
			{
			printf("MOV V%01x %02x", secondNib, code[1]);
			}
			break;
		case 0x07:
			printf("ADD V%01x %02x", secondNib, code[1]);
			break;
		case 0x08:
			{
				//unsigned char lastNib = code[1] & 0x0f;
				//unsigned char thirdNib = code[1] >> 4;
				//printf("  %01x %02x ", thirdNib, code[1]);
				switch(lastNib){
					case 0:
						printf("MOV V%01x V%01x", secondNib, thirdNib);
						break;
					case 1:
						printf("OR V%01x V%01x", secondNib, thirdNib);
						break;
					case 2:
						printf("AND V%01x V%01x", secondNib, thirdNib);
						break;
					case 3:
						printf("XOR V%01x V%01x", secondNib, thirdNib);
						break;
					case 4:
						printf("ADD V%01x V%01x", secondNib, thirdNib);
						break;
					case 5:
						printf("SUB V%01x V%01x", secondNib, thirdNib);
						break;
					case 6:
						printf("SHR V%01x V%01x", secondNib, thirdNib);
						break;
					case 7:
						printf("SUBN V%01x V%01x", secondNib, thirdNib);
						break;
					case 0xe:
						printf("SHL V%01x V%01x", secondNib, thirdNib);
						break;
				}
			}
			break;
		case 0x09:
			printf("SNE %01x %01x", secondNib, code[1]);
			break;
		case 0x0a:
			{
			printf("MVI %01x%02x", secondNib, code[1]);
			}
			break;
		case 0x0b:
			printf("JMP V0 %02x", code[1]);
			break;
		case 0x0c:
			printf("RND %02x", code[1]);
			break;
		case 0x0d:
			printf("DRW V%01x V%01x %01x", secondNib, thirdNib, lastNib);
			break;
		case 0x0e:
			switch(code[1]){
				case 0x9e:
					printf("SKP V%01x", secondNib);
					break;
				case 0xa1:
					printf("SKNP V%01x", secondNib);
					break;
			}
			break;
		case 0x0f:
			switch(code[1]){
				case 0x07:
					printf("MOV V%01x DT", secondNib);
					break;
				case 0x0A:
					printf("MOV V%01x K", secondNib);
					break;
				case 0x15:
					printf("MOV DT V%01x", secondNib);
					break;
				case 0x18:
					printf("MOV ST V%01x", secondNib);
					break;
				case 0x1e:
					printf("ADD I V%01x", secondNib);
					break;
				case 0x29:
					printf("MOV I V%01x", secondNib);
					break;
				case 0x33:
					printf("MOV B V%01x", secondNib);
					break;
				case 0x55:
					printf("MOV [I] V%01x", secondNib);
					break;
				case 0x65:
					printf("MOV V%01x [I]", secondNib);
					break;
			}
			break;
	}
}


int main(int argc, char *argv[]){
	FILE* fp = fopen(argv[1], "rb");
	if(fp == NULL){
		printf("Error could not read file.");
		exit(1);
	}

	//Goto end of file and get length of file
	//then go back to start of file
	fseek(fp, 0, SEEK_END);
	int fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);


	unsigned char *buffer = malloc(fileSize+0x200);
	fread(buffer+0x200, 1, fileSize, fp);
	fclose(fp);

	int pc = 0x200;
	while(pc < (fileSize+0x200)){
		DisassembleCH8(buffer, pc);
		pc += 2;
		printf("\n");
	}

	fclose(fp);	
	return 0;
}

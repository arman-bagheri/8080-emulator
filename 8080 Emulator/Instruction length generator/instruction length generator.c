#include <stdio.h>
#include <stdlib.h>

int lengthof(unsigned char); // 8 bit opcode to its length, how many bytes

int main(){

	FILE* fptr;
	fptr = fopen("lengths.txt","w");
	unsigned char opcode;
	fprintf(fptr, "hello files world!\n");
	fprintf(fptr, "what a wonderful world!\n");

	fprintf(fptr, "uint8_t instruction_lengths[256] = {\n");

	for(int i=0; i<32; i++){
		fprintf(fptr, "\t");
		for(int j=0; j<8; j++){
			opcode = (unsigned char)((i<<3)|j);
			fprintf(fptr, "%d,",lengthof(opcode));
		}
		fprintf(fptr, "\n");
	}

	fprintf(fptr, "};\n");

	fclose(fptr);

	return 0;
}


int lengthof(unsigned char opcode){
	int length;
	if(opcode==0x00){
		length=1;
	}
	else if((opcode&0xcf)==0x01){
		length=3;
	}
	else if((opcode&0xce)==0x02){
		length=1;
	}
	else if((opcode&0xc6)==0x04){
		length=1;
	}
	else if((opcode&0xc7)==0x06){
		length=2;
	}
	else if(opcode==0x22){
		length=3;
	}
	else if(opcode==0x2a){
		length=3;
	}
	else if(opcode==0x32){
		length=3;
	}
	else if(opcode==0x3a){
		length=3;
	}
	else if((opcode&0xc7)==0xc2){
		length=3;
		
	}
	else if(opcode==0xc3){
		length=3;
		
	}
	else if((opcode&0xc7)==0xc4){
		length=3;
	}
	else if((opcode&0xc7)==0xc6){
		length=2;
	}
	else if(opcode==0xcd){
		length=3;
	}
	else if(opcode==0xd3){
		length=2;
	}
	else if(opcode==0xdd){
		length=2;
	}
	else{
		length=1;
	}


	return length;	
}
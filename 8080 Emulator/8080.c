#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

//memory



#define memory_size (64 * 1024)
#define MEM(addr) memory[(uint16_t)(addr)]
unsigned char memory[memory_size];


// 8080 instruction length lookup table
uint8_t instruction_lengths[256] = {
    1,3,1,1,1,1,2,1,
	1,1,1,1,1,1,2,1,
	1,3,1,1,1,1,2,1,
	1,1,1,1,1,1,2,1,
	1,3,1,1,1,1,2,1,
	1,1,3,1,1,1,2,1,
	1,3,1,1,1,1,2,1,
	1,1,3,1,1,1,2,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,3,3,3,1,2,1,
	1,1,3,1,3,3,2,1,
	1,1,3,2,3,1,2,1,
	1,1,3,1,3,2,2,1,
	1,1,3,1,3,1,2,1,
	1,1,3,1,3,1,2,1,
	1,1,3,1,3,1,2,1,
	1,1,3,1,3,1,2,1
};

// registers

unsigned char A = 0, B = 0, C = 0, D = 0, E = 0, H = 0, L = 0;
uint16_t sp = 0xfffe;
uint16_t pc;
unsigned char F = 0;

//instruction

int halted = 0;

unsigned char opcode;
unsigned char operand1;
unsigned char operand2;
uint8_t instruction_length;

//register selection

unsigned char *regs[] = {&B,&C,&D,&E,&H,&L,NULL,&A};


//IO devices



void fetch_8080(){
	opcode = MEM(pc);
	instruction_length = instruction_lengths[opcode];

	printf("fetching..\n");
	printf("opcode is %x\n",opcode);
	printf("instruction length is %x\n",instruction_length);

	if (instruction_length >= 2) {
		operand1 = MEM(pc + 1);
		printf("operand1 is %x\n", operand1);
	}
	if (instruction_length == 3) {
		operand2 = MEM(pc + 2);
		printf("operand2 is %x\n", operand2);
	}
	pc += instruction_length;

	if (instruction_length == 0){
		printf("Invalid instruction %x\n",opcode);
		exit(1);
	}
	printf("fetching over..\n");

}

void execute_8080(){
	if(opcode == 0x00){              //NOP

	}
	else if((opcode&0xcf)==0x01){ //LXI
		switch((opcode^0x01) >> 4){
			case 0x00:
				B = operand2;
				C = operand1;
				break;
			case 0x01:
				D = operand2;
				E = operand1;
				break;
			case 0x02:
				H = operand2;
				L = operand1;
				break;
			case 0x03:
				sp = (uint16_t)operand1 | (((uint16_t)operand2)<<8);
				break;			
		}
	}
	else if (opcode == 0x02) {
    	MEM((B << 8) | C) = A;
} 
	else if (opcode == 0x12) {
    	MEM((D << 8) | E) = A;
}
	else if((opcode & 0xc3) == 0x03){
		switch(opcode){
			case 0x03:
				C++;
				if (C==0){
					B = B+1;
				}
				break;
			case 0x13:
				E = E+1;
				if (E==0){
					D = D+1;
				}
				break;
			case 0x23:
				L = L+1;
				if (L==0){
					H = H+1;
				}
				break;
			case 0x33:
				sp = sp + 1;
				break;			 
		}
	}
	else if ((opcode & 0x07) == 0x04){   //INR
		switch(opcode){
			case 0x04:
				B++;
				if(B>0x7f){
				//	printf("negative");
					F = F | 0x80;
				}
				else{
					F = F & 0x7f;
				}

				if(B==0){
				//	printf("zero");
					F = F | 0x40;
				}
				else{
					F = F & 0xbf;
				}

				if(__builtin_parity(B)){
				//	printf("parity yes\n");
					F = F | 0x04;
				}
				else {
					F = F & 0xfb;
				}

				if((B&0x0f)==0X00){
				//	printf("half carry\n");
					F = F | 0x10;
				}
				else {
					F = F & 0xef;
				}
				break;
			case 0x0c:
				C++;
				if(C>0x7f){
					F = F | 0x80;
				}
				else{
					F = F & 0x7f;
				}

				if(C==0){
					F = F | 0x40;
				}
				else{
					F = F & 0xbf;
				}

				if(__builtin_parity(C)){
					F = F | 0x04;
				}
				else {
					F = F & 0xfb;
				}

				if((C&0x0f)==0X00){
					F = F | 0x10;
				}
				else {
					F = F & 0xef;
				}
				break;
			case 0x14:
				D++;
				if(D>0x7f){
					F = F | 0x80;
				}
				else{
					F = F & 0x7f;
				}

				if(D==0){
					F = F | 0x40;
				}
				else{
					F = F & 0xbf;
				}

				if(__builtin_parity(D)){
					F = F | 0x04;
				}
				else {
					F = F & 0xfb;
				}

				if((D&0x0f)==0X00){
					F = F | 0x10;
				}
				else {
					F = F & 0xef;
				}
				break;
			case 0x1c:
				E++;
				if(E>0x7f){
					F = F | 0x80;
				}
				else{
					F = F & 0x7f;
				}

				if(E==0){
					F = F | 0x40;
				}
				else{
					F = F & 0xbf;
				}

				if(__builtin_parity(E)){
					F = F | 0x04;
				}
				else {
					F = F & 0xfb;
				}

				if((E&0x0f)==0X00){
					F = F | 0x10;
				}
				else {
					F = F & 0xef;
				}
				break;
			case 0x24:
				H++;
				if(H>0x7f){
					F = F | 0x80;
				}
				else{
					F = F & 0x7f;
				}

				if(H==0){
					F = F | 0x40;
				}
				else{
					F = F & 0xbf;
				}

				if(__builtin_parity(H)){
					F = F | 0x04;
				}
				else {
					F = F & 0xfb;
				}

				if((H&0x0f)==0X00){
					F = F | 0x10;
				}
				else {
					F = F & 0xef;
				}
				break;
			case 0x2c:
				L++;
				if(L>0x7f){
					F = F | 0x80;
				}
				else{
					F = F & 0x7f;
				}

				if(L==0){
					F = F | 0x40;
				}
				else{
					F = F & 0xbf;
				}

				if(__builtin_parity(L)){
					F = F | 0x04;
				}
				else {
					F = F & 0xfb;
				}

				if((L&0x0f)==0X00){
					F = F | 0x10;
				}
				else {
					F = F & 0xef;
				}
				break;
			case 0x34:
				MEM((H<<8) | L) = MEM((H<<8) | L) + 1;
				if(MEM((H<<8) | L)>0x7f){
					F = F | 0x80;
				}
				else{
					F = F & 0x7f;
				}

				if(MEM((H<<8) | L)==0){
					F = F | 0x40;
				}
				else{
					F = F & 0xbf;
				}

				if(__builtin_parity(MEM((H<<8) | L))){
					F = F | 0x04;
				}
				else {
					F = F & 0xfb;
				}

				if((MEM((H<<8) | L)&0x0f)==0X00){
					F = F | 0x10;
				}
				else {
					F = F & 0xef;
				}
				break;
			case 0x3c:
				A++;
				if(A>0x7f){
					F = F | 0x80;
				}
				else{
					F = F & 0x7f;
				}

				if(A==0){
					F = F | 0x40;
				}
				else{
					F = F & 0xbf;
				}

				if(__builtin_parity(A)){
					F = F | 0x04;
				}
				else {
					F = F & 0xfb;
				}

				if((A&0x0f)==0X00){
					F = F | 0x10;
				}
				else {
					F = F & 0xef;
				}
				break;							
		}
	}

	else if ((opcode & 0x07) == 0x05){   //DCR
		switch(opcode){
			case 0x05:
				B--;
				if(B>0x7f){
					F = F | 0x80;
				}
				else{
					F = F & 0x7f;
				}

				if(B==0){
					F = F | 0x40;
				}
				else{
					F = F & 0xbf;
				}

				if(__builtin_parity(B)){
					F = F | 0x04;
				}
				else {
					F = F & 0xfb;
				}

				if((B&0x0f)==0x0f){
					F = F | 0x10;
				}
				else {
					F = F & 0xef;
				}
				break;
			case 0x0d:
				C--;
				if(C>0x7f){
					F = F | 0x80;
				}
				else{
					F = F & 0x7f;
				}

				if(C==0){
					F = F | 0x40;
				}
				else{
					F = F & 0xbf;
				}

				if(__builtin_parity(C)){
					F = F | 0x04;
				}
				else {
					F = F & 0xfb;
				}

				if((C&0x0f)==0X0f){
					F = F | 0x10;
				}
				else {
					F = F & 0xef;
				}
				break;
			case 0x15:
				D--;
				if(D>0x7f){
					F = F | 0x80;
				}
				else{
					F = F & 0x7f;
				}

				if(D==0){
					F = F | 0x40;
				}
				else{
					F = F & 0xbf;
				}

				if(__builtin_parity(D)){
					F = F | 0x04;
				}
				else {
					F = F & 0xfb;
				}

				if((D&0x0f)==0X0f){
					F = F | 0x10;
				}
				else {
					F = F & 0xef;
				}
				break;
			case 0x1d:
				E--;
				if(E>0x7f){
					F = F | 0x80;
				}
				else{
					F = F & 0x7f;
				}

				if(E==0){
					F = F | 0x40;
				}
				else{
					F = F & 0xbf;
				}

				if(__builtin_parity(E)){
					F = F | 0x04;
				}
				else {
					F = F & 0xfb;
				}

				if((E&0x0f)==0X0f){
					F = F | 0x10;
				}
				else {
					F = F & 0xef;
				}
				break;
			case 0x25:
				H--;
				if(H>0x7f){
					F = F | 0x80;
				}
				else{
					F = F & 0x7f;
				}

				if(H==0){
					F = F | 0x40;
				}
				else{
					F = F & 0xbf;
				}

				if(__builtin_parity(H)){
					F = F | 0x04;
				}
				else {
					F = F & 0xfb;
				}

				if((H&0x0f)==0X0f){
					F = F | 0x10;
				}
				else {
					F = F & 0xef;
				}
				break;
			case 0x2d:
				L--;
				if(L>0x7f){
					F = F | 0x80;
				}
				else{
					F = F & 0x7f;
				}

				if(L==0){
					F = F | 0x40;
				}
				else{
					F = F & 0xbf;
				}

				if(__builtin_parity(L)){
					F = F | 0x04;
				}
				else {
					F = F & 0xfb;
				}

				if((L&0x0f)==0X0f){
					F = F | 0x10;
				}
				else {
					F = F & 0xef;
				}
				break;
			case 0x35:
				MEM((H<<8) | L) = MEM((H<<8) | L) - 1;
				if(MEM((H<<8) | L)>0x7f){
					F = F | 0x80;
				}
				else{
					F = F & 0x7f;
				}

				if(MEM((H<<8) | L)==0){
					F = F | 0x40;
				}
				else{
					F = F & 0xbf;
				}

				if(__builtin_parity(MEM((H<<8) | L))){
					F = F | 0x04;
				}
				else {
					F = F & 0xfb;
				}

				if((MEM((H<<8) | L)&0x0f)==0X0f){
					F = F | 0x10;
				}
				else {
					F = F & 0xef;
				}
				break;
			case 0x3d:
				A--;
				if(A>0x7f){
					F = F | 0x80;
				}
				else{
					F = F & 0x7f;
				}

				if(A==0){
					F = F | 0x40;
				}
				else{
					F = F & 0xbf;
				}

				if(__builtin_parity(A)){
					F = F | 0x04;
				}
				else {
					F = F & 0xfb;
				}

				if((A&0x0f)==0X0f){
					F = F | 0x10;
				}
				else {
					F = F & 0xef;
				}
				break;							
		}
	}

	else if ((opcode & 0xc7)==0x06){  //MVI
		printf("hello mvi\n");
		if(regs[(opcode>>3)]==NULL){
			MEM((H<<8)|L) = operand1;
		}
		else {
			*regs[(opcode>>3)] = operand1;
		}
	}
	else if((opcode & 0xcf)==0x09){      //DAD
		uint16_t HL = (H << 8) | L;
    	uint16_t RP = 0;

    switch (opcode) {
        case 0x09: // DAD B
            RP = (B << 8) | C;
            break;
        case 0x19: // DAD D
            RP = (D << 8) | E;
            break;
        case 0x29: // DAD H
            RP = (H << 8) | L;
            break;
        case 0x39: // DAD SP
            RP = sp;
            break;
            
    }
    uint16_t HL2 = RP + HL;
    L = (unsigned char)HL2;
    H = (unsigned char)(HL2 >> 8);

    if((HL2 >= HL) && (HL2 >= RP)){
    	F = F & 0xfe;
    }
    else {
    	F = F | 0x01;
    }

}


	else if((opcode & 0xcf)==0x0a){  //LDAX
		switch(opcode){
			case 0x0a:
				A = MEM((B<<8) | C);
				break;
			case 0x1a:
				A = MEM((D<<8) | E);
				break;
			case 0x2a:
				printf("Invalid LDAX Instruction!");
				exit(1);
				break;
			case 0x3a:
				printf("Invalid LDAX Instruction!");
				exit(1);
				break;			
		}
	}
	else if((opcode & 0xcf)==0x0b){ //DCX
		uint16_t mid = 0;
		switch(opcode){
			case 0x0b:
				mid = (B<<8) | C;
				mid = mid - 1;
				B = (unsigned char)(mid>>8);
				C = (unsigned char)(mid&0x00ff);
				break;
			case 0x1b:
				mid = (D<<8) | E;
				mid = mid - 1;
				D = (unsigned char)(mid>>8);
				E = (unsigned char)(mid&0x00ff);
				break;
			case 0x2b:
				mid = (H<<8) | L;
				mid = mid - 1;
				H = (unsigned char)(mid>>8);
				L = (unsigned char)(mid&0x00ff);
				break;
			case 0x3b:
				sp--;
				break;			
		}
	}
	else if(opcode==0x07){  //RLC
		F = (F & 0xfe) | (A>>7);
		A = (A<<1) | (A>>7);
}
	else if(opcode==0x0f){  //RRC
		F = (F & 0xfe) | (A & 0x01);
		A = (A>>1) | (A<<7);
	}


	else if(opcode==0x17){	//RAL
		if(F&0x01){
			F = (F & 0xfe) | (A>>7);
			A = (A<<1) | 0x01;	
		}
		else {
			F = (F & 0xfe) | (A>>7);
			A = (A<<1);
		}
	}

	else if(opcode==0x1f){	//RAR 
		if(F&0x01){
			F = (F&0xfe) | (A&0x01);
			A = (A>>7) | 0x80;
		}
		else{
			F = (F&0xfe) | (A&0x01);
			A = (A>>7);
		}
	}

	else if(opcode==0x22){ //SHLD
		MEM((operand2<<8)|operand1) = L;
		MEM(((operand2<<8)|operand1)+1) = H;
	}


	else if(opcode==0x27){ //DAA
    unsigned char correction = 0;
    unsigned char oldA = A;
    unsigned char carry = F & 0x01;       // Carry flag (CY)
    unsigned char auxCarry = (F >> 4) & 0x01;  // Auxiliary Carry (AC)
    
    // If lower nibble > 9 or AC set, add 0x06
    if (((A & 0x0F) > 9) || auxCarry) {
        correction += 0x06;
    }
    // If upper nibble > 9 or CY set, add 0x60
    if (((A >> 4) > 9) || carry) {
        correction += 0x60;
        F |= 0x01;  // Set carry flag
    } else {
        F &= 0xFE;  // Clear carry flag
    }

    unsigned short result = (unsigned short)A + correction;
    A = (unsigned char)(result & 0xFF);

    // Update Auxiliary Carry flag:
    // If there was a carry from bit 3 to bit 4 during the correction,
    // set AC; otherwise clear it.
    if (((oldA ^ A) & 0x10) != 0) {
        F |= 0x10; // Set AC flag
    } else {
        F &= 0xEF; // Clear AC flag
    }

    // Update other flags (Zero, Sign, Parity) accordingly
    // For example:
    // set_zero_flag(A);
    // set_sign_flag(A);
    // set_parity_flag(A);
	}


	else if(opcode==0x2a){  //LHLD
		uint16_t mid = 0;
		mid = (operand2<<8)|operand1;
		L = MEM(mid);
		H = MEM(mid+1);
	}
	else if(opcode==0x2f){ //CMA
		A = ~A;
	}
	else if(opcode==0x32){ //STA
		MEM((operand2<<8)|operand1)=A;
	}
	

	else if(opcode==0x34){//STC
		F = F | 0x01;
	}

	else if(opcode==0x3a){ //LDA
		A = MEM((operand2<<8)|operand1);
	}
	

	else if(opcode==0x3f){  //CMC
		F = (F&0xfe) | ((~F)&0x01);
	}

	else if(opcode==0x76){         //MOV　invalid and HLt
		printf("halted at 0x%x\n",pc);
		halted=1;
	}   
	else if((opcode&0xf8)==0x70){	//MOV memory destination
		MEM((H<<8)|L) = *regs[(opcode&0x07)];
	}
	else if((opcode&0xc7)==0x46){	//MOV memory source
		*regs[(opcode&0x38)>>3] = MEM((H<<8)|L);
	}
	else if((opcode&0xc0)==0x40){	//MOV no memory
		*regs[(opcode&0x38)>>3] = *regs[(opcode&0x07)];
	}

	else if ((opcode & 0xC0) == 0x80) { // ALU group
    unsigned char *src;
    uint8_t result;
    uint8_t carry = F & 0x01;
    uint8_t operand;

    src = regs[opcode & 0x07];
    operand = (src == NULL) ? MEM((H << 8) | L) : *src;

    switch ((opcode >> 3) & 0x07) {
        case 0x00: // ADD
            result = A + operand;
            F = (result < A) ? (F | 0x01) : (F & 0xFE); // Carry
            F = (((A & 0x0F) + (operand & 0x0F)) > 0x0F) ? (F | 0x10) : (F & 0xEF); // Aux carry
            A = result;
            break;

        case 0x01: // ADC
            result = A + operand + carry;
            F = (result < A || (carry && result == A)) ? (F | 0x01) : (F & 0xFE);
            F = (((A & 0x0F) + (operand & 0x0F) + carry) > 0x0F) ? (F | 0x10) : (F & 0xEF);
            A = result;
            break;

        case 0x02: // SUB
            result = A - operand;
            F = (A < operand) ? (F | 0x01) : (F & 0xFE);
            F = ((A & 0x0F) < (operand & 0x0F)) ? (F | 0x10) : (F & 0xEF);
            A = result;
            break;

        case 0x03: // SBB
            result = A - operand - carry;
            F = (A < operand + carry) ? (F | 0x01) : (F & 0xFE);
            F = ((A & 0x0F) < ((operand & 0x0F) + carry)) ? (F | 0x10) : (F & 0xEF);
            A = result;
            break;

        case 0x04: // ANA
            A = A & operand;
            F &= 0xFE; // Clear carry
            F |= 0x10; // Set aux carry by convention
            break;

        case 0x05: // XRA
            A = A ^ operand;
            F &= 0xEE; // Clear carry and aux
            break;

        case 0x06: // ORA
            A = A | operand;
            F &= 0xEE; // Clear carry and aux
            break;

        case 0x07: // CMP
            result = A - operand;
            F = (A < operand) ? (F | 0x01) : (F & 0xFE);
            F = ((A & 0x0F) < (operand & 0x0F)) ? (F | 0x10) : (F & 0xEF);
            break;
    }

    // Set Zero flag
    if (((opcode >> 3) & 0x07) == 0x07)  // CMP
        result = A - operand;
    else
        result = A;

    F = (result == 0) ? (F | 0x40) : (F & 0xBF);     // Zero
    F = (result & 0x80) ? (F | 0x80) : (F & 0x7F);   // Sign
    F = (__builtin_parity(result)) ? (F & 0xFB) : (F | 0x04); // Parity (even parity = 0)
    
    if (((opcode >> 3) & 0x07) == 0x07)  // CMP
        A = A; // no change
}

	else if(opcode==0xc0){    //conditional returns
		if((F&0x40)==0x00){
			pc = (MEM(sp+1)<<8) | MEM(sp);
						sp = sp + 2;
		}
	}

	else if(opcode==0xc8){
		if((F&0x40)==0x40){
			pc = (MEM(sp+1)<<8) | MEM(sp);
						sp = sp + 2;
		}
	}
	else if(opcode==0xd0){
		if((F&0x01)==0x00){
			pc = (MEM(sp+1)<<8) | MEM(sp);
						sp = sp + 2;
		}
	}
	else if(opcode==0xd8){
		if((F&0x01)==0x01){
			pc = (MEM(sp+1)<<8) | MEM(sp);
						sp = sp + 2;
		}
	}
	else if(opcode==0xe0){
		if((F&0x04)==0x00){
			pc = (MEM(sp+1)<<8) | MEM(sp);
						sp = sp + 2;
		}
	}
	else if(opcode==0xe8){
		if((F&0x04)==0x04){
			pc = (MEM(sp+1)<<8) | MEM(sp);
						sp = sp + 2;
		}
	}
	else if(opcode==0xf0){
		if((F&0x80)==0x00){
			pc = (MEM(sp+1)<<8) | MEM(sp);
						sp = sp + 2;
		}
	}
	else if(opcode==0xf8){
		if((F&0x80)==0x80){
			pc = (MEM(sp+1)<<8) | MEM(sp);
			sp = sp + 2;
		}
	}			


else if(opcode == 0xc5) {  // PUSH B
    sp -= 2;
    MEM(sp) = C;
    MEM(sp + 1) = B;
}

else if(opcode == 0xd5) {  // PUSH D
    sp -= 2;
    MEM(sp) = E;
    MEM(sp + 1) = D;
}

else if(opcode == 0xe5) {  // PUSH H
    sp -= 2;
    MEM(sp) = L;
    MEM(sp + 1) = H;
}

else if(opcode == 0xf5) {  // PUSH PSW (A and F)
    sp -= 2;
    MEM(sp) = F;
    MEM(sp + 1) = A;
}



else if(opcode == 0xc1) {  // POP B
    C = MEM(sp);
    B = MEM(sp + 1);
    sp += 2;
}

else if(opcode == 0xd1) {  // POP D
    E = MEM(sp);
    D = MEM(sp + 1);
    sp += 2;
}

else if(opcode == 0xe1) {  // POP H
    L = MEM(sp);
    H = MEM(sp + 1);
    sp += 2;
}

else if(opcode == 0xf1) {  // POP PSW (A and F)
    F = MEM(sp);
    A = MEM(sp + 1);
    sp += 2;
}


else if(opcode==0xc2){  //jump conditionals
	if((F&0x40)==0x00){
		pc = (operand2<<8)|operand1;
	}
}
else if(opcode==0xca){  //jump conditionals
	if((F&0x40)==0x40){
		pc = (operand2<<8)|operand1;
	}
}
else if(opcode==0xd2){  //jump conditionals
	if((F&0x01)==0x00){
		pc = (operand2<<8)|operand1;
	}
}
else if(opcode==0xda){  //jump conditionals
	if((F&0x01)==0x01){
		pc = (operand2<<8)|operand1;
	}
}
else if(opcode==0xe2){  //jump conditionals
	if((F&0x04)==0x00){
		pc = (operand2<<8)|operand1;
	}
}
else if(opcode==0xea){  //jump conditionals
	if((F&0x04)==0x04){
		pc = (operand2<<8)|operand1;
	}
}
else if(opcode==0xf2){  //jump conditionals
	if((F&0x80)==0x00){
		pc = (operand2<<8)|operand1;
	}
}
else if(opcode==0xfa){  //jump conditionals
	if((F&0x80)==0x80){
		pc = (operand2<<8)|operand1;
	}
}


else if(opcode==0xc3){  //JUMP
	pc = (operand2<<8)|operand1;
}



else if(opcode==0xc4){  //call conditionals
	if((F&0x40)==0x00){
		sp = sp - 2;
		MEM(sp) = (unsigned char) (pc&0xff);
		MEM(sp+1) = (unsigned char) ((pc>>8)&0xff);
		pc = (operand2<<8)|operand1;
	}
}
else if(opcode==0xcc){  //call conditionals
	if((F&0x40)==0x40){
		sp = sp - 2;
		MEM(sp) = (unsigned char) (pc&0xff);
		MEM(sp+1) = (unsigned char) ((pc>>8)&0xff);
		pc = (operand2<<8)|operand1;	}
}
else if(opcode==0xd4){  //call conditionals
	if((F&0x01)==0x00){
		sp = sp - 2;
		MEM(sp) = (unsigned char) (pc&0xff);
		MEM(sp+1) = (unsigned char) ((pc>>8)&0xff);
		pc = (operand2<<8)|operand1;
	}
}
else if(opcode==0xdc){  //call conditionals
	if((F&0x01)==0x01){
		sp = sp - 2;
		MEM(sp) = (unsigned char) (pc&0xff);
		MEM(sp+1) = (unsigned char) ((pc>>8)&0xff);
		pc = (operand2<<8)|operand1;
	}
}
else if(opcode==0xe4){  //call conditionals
	if((F&0x04)==0x00){
		sp = sp - 2;
		MEM(sp) = (unsigned char) (pc&0xff);
		MEM(sp+1) = (unsigned char) ((pc>>8)&0xff);
		pc = (operand2<<8)|operand1;
	}
}
else if(opcode==0xec){  //call conditionals
	if((F&0x04)==0x04){
		sp = sp - 2;
		MEM(sp) = (unsigned char) (pc&0xff);
		MEM(sp+1) = (unsigned char) ((pc>>8)&0xff);
		pc = (operand2<<8)|operand1;
	}
}
else if(opcode==0xf4){  //call conditionals
	if((F&0x80)==0x00){
		sp = sp - 2;
		MEM(sp) = (unsigned char) (pc&0xff);
		MEM(sp+1) = (unsigned char) ((pc>>8)&0xff);
		pc = (operand2<<8)|operand1;
	}
}
else if(opcode==0xfc){  //call conditionals
	if((F&0x80)==0x80){
		sp = sp - 2;
		MEM(sp) = (unsigned char) (pc&0xff);
		MEM(sp+1) = (unsigned char) ((pc>>8)&0xff);
		pc = (operand2<<8)|operand1;
	}
}

else if(opcode==0xc6){   //ADI
	unsigned char oldA = A;
	A = A + operand1;
	if(A==0){
		F = F | 0x40;
	}
	else {
		F = F & 0xbf;
	}
	if (A>>7){  //sign
		F = F | 0x80;
	}
	else {
		F = F & 0x7f;
	}

	F = (__builtin_parity(A)) ? (F & 0xFB) : (F | 0x04);

	if((oldA + operand1)>A){	//carry
		F = F | 0x01;
	}
	else {
		F = F & 0xfe;
	}

	if(((oldA&0x0f)+(operand1&0x0f))>(A&0x0f)){   //half carry
		F = F | 0x10;
	}
	else {
		F = F & 0xef;
	}
}

else if(opcode==0xc6){   //ACI
	unsigned char oldA = A;
	A = A + operand1 + (F&0x01);
	if(A==0){
		F = F | 0x40;
	}
	else {
		F = F & 0xbf;
	}
	if (A>>7){  //sign
		F = F | 0x80;
	}
	else {
		F = F & 0x7f;
	}

	F = (__builtin_parity(A)) ? (F & 0xFB) : (F | 0x04);

	if((oldA + operand1 + (F&0x01))>A){	//carry
		F = F | 0x01;
	}
	else {
		F = F & 0xfe;
	}

	if(((oldA&0x0f)+ (operand1&0x0f) + (F&0x01))>(A&0x0f)){   //half carry
		F = F | 0x10;
	}
	else {
		F = F & 0xef;
	}
}

else if(opcode==0xc6){   //SUI
	unsigned char oldA = A;
	A = A - operand1;
	if(A==0){
		F = F | 0x40;
	}
	else {
		F = F & 0xbf;
	}
	if (A>>7){  //sign
		F = F | 0x80;
	}
	else {
		F = F & 0x7f;
	}

	F = (__builtin_parity(A)) ? (F & 0xFB) : (F | 0x04);

	if((oldA - operand1)>A){	//carry
		F = F | 0x01;
	}
	else {
		F = F & 0xfe;
	}

	if(((oldA&0x0f)-(operand1&0x0f))>(A&0x0f)){   //half carry
		F = F | 0x10;
	}
	else {
		F = F & 0xef;
	}
}

else if(opcode==0xc6){   //SBI
	unsigned char oldA = A;
	A = A - operand1 - (F&0x01);
	if(A==0){
		F = F | 0x40;
	}
	else {
		F = F & 0xbf;
	}
	if (A>>7){  //sign
		F = F | 0x80;
	}
	else {
		F = F & 0x7f;
	}

	F = (__builtin_parity(A)) ? (F & 0xFB) : (F | 0x04);

	if((oldA - operand1 - (F&0x01))>A){	//carry
		F = F | 0x01;
	}
	else {
		F = F & 0xfe;
	}

	if(((oldA&0x0f)- (operand1&0x0f) - (F&0x01))>(A&0x0f)){   //half carry
		F = F | 0x10;
	}
	else {
		F = F & 0xef;
	}
}

else if(opcode==0xc6){   //ANI
	A = A & operand1;
	if(A==0){
		F = F | 0x40;
	}
	else {
		F = F & 0xbf;
	}
	if (A>>7){  //sign
		F = F | 0x80;
	}
	else {
		F = F & 0x7f;
	}

	F = (__builtin_parity(A)) ? (F & 0xFB) : (F | 0x04);

}

else if(opcode==0xc6){   //XRI
	A = A ^ operand1;
	if(A==0){
		F = F | 0x40;
	}
	else {
		F = F & 0xbf;
	}
	if (A>>7){  //sign
		F = F | 0x80;
	}
	else {
		F = F & 0x7f;
	}

	F = (__builtin_parity(A)) ? (F & 0xFB) : (F | 0x04);

}

else if(opcode==0xc6){   //ORI
	A = A | operand1;
	if(A==0){
		F = F | 0x40;
	}
	else {
		F = F & 0xbf;
	}
	if (A>>7){  //sign
		F = F | 0x80;
	}
	else {
		F = F & 0x7f;
	}

	F = (__builtin_parity(A)) ? (F & 0xFB) : (F | 0x04);

}

else if(opcode==0xc6){   //CPI
	unsigned char newA = A - operand1;
	if(A==0){
		F = F | 0x40;
	}
	else {
		F = F & 0xbf;
	}
	if (A>>7){  //sign
		F = F | 0x80;
	}
	else {
		F = F & 0x7f;
	}

	F = (__builtin_parity(A)) ? (F & 0xFB) : (F | 0x04);

	if ((A-operand1)>newA){
		F = F | 0x01;
	}
	else {
		F = F & 0xfe;
	}
	if(((A&0x0f)-(operand1&0x0f))>(newA&0x0f)){
		F = F | 0x10;
	}
	else {
		F = F & 0xef;
	}

}

else if((opcode&0xc7)==0xc7){ //RST n
	sp -= 2;
	MEM(sp) = (unsigned char)(pc&0x00ff);
	MEM(sp+1) = (unsigned char)(pc>>8);
	pc = opcode & 0x38;
}

else if(opcode==0xc9){ //RET
	pc = MEM(sp) | (MEM(sp+1)<<8);
	sp += 2;
}

else if(opcode==0xcd){ //CALL
	sp -= 2;
	MEM(sp) = (unsigned char)(pc&0x00ff);
	MEM(sp+1) = (unsigned char)(pc>>8);
	pc = operand1 | (operand2<<8);	
}

// OUT and IN remain  and interrupt mechanics




else if(opcode==0xe3){ //XTHL
	uint16_t HL = (H<<8) | L;
	L = MEM(sp);
	H = MEM(sp+1);
	MEM(sp) = (HL & 0x00ff);
	MEM(sp+1) = (HL >> 8);
}

else if(opcode==0xe9){ //PCHL
	uint16_t HL = (H<<8) | L;
	L = (unsigned char)(pc&0x00ff);
	H = (unsigned char)(pc>>8);
	pc = HL;
}

else if(opcode==0xeb){ //XCHG
	unsigned char oldD = D;
	unsigned char oldE = E;
	D = H;
	E = L;
	H = oldD;
	L = oldE;
}

else if(opcode==0xf9){ // SPHL
	sp = (H<<8) | L;
}




	else {   //unrecognized opcode
    printf("Unrecognized opcode: 0x%02x\n", opcode);
    exit(1);
}
}



void print_state(){
	printf("A is %x\n",A);
	printf("B is %x\n",B);
	printf("C is %x\n",C);
	printf("D is %x\n",D);
	printf("E is %x\n",E);
	printf("H is %x\n",H);
	printf("L is %x\n",L);
	printf("the flags are  0x%08b\n",F);
	printf("pc is %x\n",pc);
	printf("sp is %x\n\n",sp);
}

void run_8080(uint16_t first_instruction){
	pc = first_instruction;
	while (!halted) {
		fetch_8080();
		execute_8080();
		print_state();
	}
}


void load_program(int start_address){
	FILE* fptr;
	fptr = fopen("program.txt","r");
	if(fptr == NULL){
		printf("The program text file not found!\n");
		exit(1);
	}
	char instruction_buffer[10];
	unsigned char adder = 0x80;
	unsigned char instruction=0;
	while(fgets(instruction_buffer,10,fptr)){
		instruction = 0;
//		printf("%s", instruction_buffer);
		for(int i=0;i<8;i++){
			if (instruction_buffer[i]=='1'){
				instruction += (adder >> i);
			}
		}
//		printf("%x\n", instruction);
		MEM(start_address) = instruction;
		start_address++;
	}

}

void print_memory(uint16_t address){
	printf("memory at address 0x%x, is 0x%x\n",address,MEM(address));
}

int main(){
	uint16_t start_instruction = 0x0000;


	load_program(0x00000000);


	run_8080(start_instruction);

	print_memory(0xb500);

	
	return 0;
}


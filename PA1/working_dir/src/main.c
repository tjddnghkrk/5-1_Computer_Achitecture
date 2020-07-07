#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

typedef unsigned char uch;

typedef struct Rtype{uch rd, rs, rt, shamt, funct;}rtype;
typedef struct Itype{uch rs, rt; short immediate;}itype;
typedef struct Jtype{unsigned int address;}jtype;

int bit_slice(unsigned int data, int left, int right);
void Rcommand(uch rs, uch rt, uch rd, uch shamt, uch funct);
void Icommand(uch rs, uch rt, short immediate, uch opcode);
void Jcommand(unsigned int address, uch opcode);

int main() {
    while (1) {
        char *p;
        char input[100];
        FILE *fp1;
        unsigned int data;
        unsigned int pdata;
        int read;
        int inst_num = 0;

        printf("mips-sim> ");
        fgets(input, sizeof(input) - 1, stdin);
        if ((p = strchr(input, '\n')) != NULL)
            *p = '\0';

        if (strncmp(input, "exit", 4) == 0) //exit
            break;
        else if (strncmp(input, "read", 4) == 0) // read
        {

            unsigned int opcode;

            char *ptr = input + 5;
            if ((fp1 = fopen(ptr, "rb")) == NULL) {
                fputs("파일 열기 에러!", stderr);
                exit(1);
            } else {
                while (read = fread(&data, sizeof(int), 1, fp1) != 0) { // read data

                    pdata = htonl(data);

                    opcode = bit_slice(pdata, 26, 31);

                    printf("inst %d: %08x ", inst_num++, pdata);

                    if (opcode == 0) { //r
                        rtype rnum;
                        rnum.rs = bit_slice(pdata, 21, 26);
                        rnum.rt = bit_slice(pdata, 16, 21);
                        rnum.rd = bit_slice(pdata, 11, 16);
                        rnum.shamt = bit_slice(pdata, 6, 11);
                        rnum.funct = bit_slice(pdata, 0, 6);
                        Rcommand(rnum.rs, rnum.rt, rnum.rd, rnum.shamt, rnum.funct);

                    } else if (opcode > 3) { 
                        itype inum;
                        inum.rs = bit_slice(pdata, 21, 26);
                        inum.rt = bit_slice(pdata, 16, 21);
                        inum.immediate = bit_slice(pdata, 0, 16);
			Icommand(inum.rs, inum.rt, inum.immediate, opcode);
			}
			 else {
                        jtype jnum;
                        jnum.address = bit_slice(pdata, 0, 26);
			Jcommand(jnum.address, opcode);  
//                      printf("x%07x", jnum.address);


                    }
		printf("\n");
                }
                fclose(fp1);
            }

        } else {
            printf("unexpected input\n"); //unexpected
            return -1;
        }
    }
    return 0;

}

int bit_slice(unsigned int data, int left, int right) {

    unsigned int range;
    unsigned int val;

    if (right == 31) {
        range = ~0;
    } else {
        range = (1 << (right - left)) - 1;
    }
    val = range & (data >> left);
    return val;
}

void Rcommand(uch rs, uch rt, uch rd, uch shamt, uch funct) {
    if (funct == 2) {
        printf("srl $%d, $%d, %d", rd, rt, shamt);
    } else if (funct == 3) {
        printf("sra $%d, $%d, %d", rd, rt, shamt);
    } else if (funct == 4) {
        printf("sllv $%d, $%d, $%d", rd, rt, rs);
    } else if (funct == 6) {
        printf("srlv $%d, $%d, $%d", rd, rt, rs);
    } else if (funct == 7) {
        printf("srav $%d, $%d, $%d", rd, rt ,rs);
    } else if (funct == 8) {
        printf("jr $%d", rs);
    } else if (funct == 9) {
        printf("jalr $%d", rs);
    } else if (funct == 12) {
        printf("syscall");
    } else if (funct == 16) {
        printf("mfhi $%d", rd);
	} else if (funct == 17){
        printf("mthi $%d", rs);
	} else if (funct == 18){
        printf("mfli $%d", rd);
	} else if (funct == 19){
        printf("mtlo $%d", rs);
	} else if (funct == 24){
	printf("mult $%d, $%d", rs, rt);
	} else if (funct == 25){
	printf("multu $%d, $%d", rs, rt);
	} else if (funct == 26){
	printf("div $%d, $%d", rs, rt);
	} else if (funct == 27){
	printf("divu $%d, $%d", rs, rt);
	} else if (funct == 32){
	printf("add $%d, $%d, $%d", rd, rs, rt);
	} else if (funct == 33){ 
	printf("addu $%d, $%d, $%d", rd, rs, rt);
	} else if (funct == 34){
	printf("sub $%d, $%d, $%d", rd, rs, rt);
	} else if (funct == 35){
	printf("subu $%d, $%d, $%d", rd, rs, rt);
	} else if (funct == 36){
	printf("and $%d, $%d, $%d", rd, rs, rt);
	} else if (funct == 37){
	printf("or $%d, $%d, $%d", rd, rs, rt);
	} else if (funct == 38){
	printf("xor $%d, $%d, $%d", rd, rs, rt);
	} else if (funct == 39){
	printf("nor $%d, $%d, $%d", rd, rs, rt);
	} else if (funct == 42){
	printf("slt $%d, $%d, $%d", rd, rs, rt);
	} else if (funct == 43){
	printf("sltu $%d, $%d, $%d", rd, rs, rt);
	} else if (funct == 0) {
        	printf("sll $%d, $%d, %d", rd, rt, shamt);
        }
	else
{	printf("unknown instruction");
}
}

void Icommand(uch rs, uch rt, short immediate, uch opcode){
    
        if (opcode == 8) {
            printf("addi $%d, $%d, %d", rt, rs, immediate);
	} else if (opcode == 9) {	
            printf("addiu $%d, $%d, %d", rt, rs, immediate);
        } else if (opcode == 12) {
            printf("andi $%d, $%d, %d", rt, rs, immediate);
	} else if (opcode == 4 ) {
            printf("beq $%d, $%d, %d", rs, rt, immediate);
        } else if (opcode == 5) {
            printf("bne $%d, $%d, %d", rs, rt, immediate);
	} else if (opcode == 32) {
            printf("lb $%d, %d($%d)", rt, immediate, rs);
	} else if (opcode == 36) {
            printf("lbu $%d, %d($%d)", rt, immediate, rs);
	} else if (opcode == 33) {
            printf("lh $%d, %d($%d)", rt, immediate, rs);
	} else if (opcode == 37) {
            printf("lhu $%d, %d($%d)", rt, immediate, rs);
	} else if (opcode == 15) {
            printf("lui $%d, %d", rt, immediate);
        } else if (opcode == 35) {
            printf("lw $%d, %d($%d)", rt, immediate, rs);
        } else if (opcode == 13) {
            printf("ori $%d, $%d, %d", rt, rs, immediate);
        } else if (opcode == 40) {
            printf("sb $%d, %d($%d)", rt, immediate, rs);
        } else if (opcode == 10) {
            printf("slti $%d, $%d, %d", rt, rs, immediate);
        } else if (opcode == 11) {
            printf("sltiu $%d, $%d, %d", rt, rs, immediate);
        } else if (opcode == 41) {
            printf("sh $%d, %d($%d)", rt, immediate, rs);
        } else if (opcode == 43) {
            printf("sw $%d, %d($%d)", rt, immediate, rs);
        } else if (opcode == 14) {
            printf("xori $%d, $%d, %d", rt, rs, immediate);
        }
	else
		printf("unknown instruction");
}
void Jcommand(unsigned int address, uch opcode){
        if (opcode == 2) {
        	printf("j %d", address);
	} else if (opcode == 3) {
        	printf("jal %d", address);
        }
	else
		printf("unknown instruction");

}


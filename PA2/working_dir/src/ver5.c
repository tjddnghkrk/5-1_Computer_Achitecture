#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

typedef unsigned char uch;

typedef struct Rtype{uch rd, rs, rt, shamt, funct;}rtype;
typedef struct Itype{uch rs, rt; short immediate;}itype;
typedef struct Jtype{unsigned int address;}jtype;

int bit_slice(unsigned int data, int left, int right);
void Rcommandread(uch rs, uch rt, uch rd, uch shamt, uch funct);
void Icommandread(uch rs, uch rt, short immediate, uch opcode);
void Jcommandread(unsigned int address, uch opcode);
void Rcommand(uch rs, uch rt, uch rd, uch shamt, uch funct);
void Icommand(uch rs, uch rt, short immediate, uch opcode);
//void Jcommand(unsigned int address, uch opcode);

unsigned int instruction[16000];
int registers[32];
int pc;


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
                        Rcommandread(rnum.rs, rnum.rt, rnum.rd, rnum.shamt, rnum.funct);

                    } else if (opcode > 3) {
                        itype inum;
                        inum.rs = bit_slice(pdata, 21, 26);
                        inum.rt = bit_slice(pdata, 16, 21);
                        inum.immediate = bit_slice(pdata, 0, 16);
                        Icommandread(inum.rs, inum.rt, inum.immediate, opcode);
                    }
                    else {
                        jtype jnum;
                        jnum.address = bit_slice(pdata, 0, 26);
                        Jcommandread(jnum.address, opcode);


                    }
                    printf("\n");
                }
                fclose(fp1);
            }

        } else if (strncmp(input, "loadinst", 8) == 0) {

            char *ptr = input + 9;
            int i = 0;
            for (int j = 0; j < 16000; j++) {
                instruction[j] = 0;
            }


            if ((fp1 = fopen(ptr, "rb")) == NULL) {
                fputs("파일 열기 에러!", stderr);
                exit(1);
            } else {
		for (int k = 0; k<16000; k++) { // read data
                    
                    read = fread(&data, sizeof(int), 1, fp1);
                    pdata = htonl(data);
                    instruction[k] = pdata;

                }
            }
		for(i = 0; i<10; i++)
            {
                printf("$0x%08x\n", instruction[i]);
            }
            fclose(fp1);
        }

        else if (strncmp(input, "run", 3) == 0){

            unsigned int opcode;
            int number;
            int i=0;
		int j = 0;
            char * ptr = input + 4;
            number = atoi(ptr); //convert to number
	
	 for(int k = 0; k<32; k++)
            {
                registers[k] = 0;
            }

            while(1)
            {
                if(i >= number) {
                    printf("Executed %d instructions\n", i);
                    break;
                }
                if(instruction[j] == 0){
                    continue;
			j++;
			}

		opcode = bit_slice(instruction[j], 26, 31);


                if (opcode == 0) { //r
                    rtype rnum;
                    rnum.rs = bit_slice(instruction[j], 21, 26);
                    rnum.rt = bit_slice(instruction[j], 16, 21);
                    rnum.rd = bit_slice(instruction[j], 11, 16);
                    rnum.shamt = bit_slice(instruction[j], 6, 11);
                    rnum.funct = bit_slice(instruction[j], 0, 6);
                    Rcommand(rnum.rs, rnum.rt, rnum.rd, rnum.shamt, rnum.funct);

                } else if (opcode > 3) {
                    itype inum;
                    inum.rs = bit_slice(instruction[j], 21, 26);
                    inum.rt = bit_slice(instruction[j], 16, 21);
                    inum.immediate = bit_slice(instruction[j], 0, 16);
                    Icommand(inum.rs, inum.rt, inum.immediate, opcode);
                }
                /*else {
                    jtype jnum;
                    jnum.address = bit_slice(pdata, 0, 26);
                    Jcommand(jnum.address, opcode);


                }*/
		j++;
                i++;
            }


        }
        else if (strncmp(input, "registers", 9) == 0){

            for(int i = 0; i<32; i++)
            {
                printf("$%d :  0x%08x\n", i, registers[i]);
            }

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

void Rcommandread(uch rs, uch rt, uch rd, uch shamt, uch funct) {
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
        printf("mflo $%d", rd);
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
    {       printf("unknown instruction");
    }
}


void Icommandread(uch rs, uch rt, short immediate, uch opcode){

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
void Jcommandread(unsigned int address, uch opcode){
    if (opcode == 2) {
        printf("j %d", address);
    } else if (opcode == 3) {
        printf("jal %d", address);
    }
    else
        printf("unknown instruction");

}

void Rcommand(uch rs, uch rt, uch rd, uch shamt, uch funct) {
    if (funct == 2) {
        registers[rd] = registers[rt] >> shamt;
        //printf("srl $%d, $%d, %d", rd, rt, shamt);
    } else if (funct == 3) {
        registers[rd] = (signed int)registers[rt] >> shamt;
        //printf("sra $%d, $%d, %d", rd, rt, shamt);
    } else if (funct == 4) {
        registers[rd] = registers[rt] << registers[rs];
        //printf("sllv $%d, $%d, $%d", rd, rt, rs);
    } else if (funct == 6) {
        registers[rd] = registers[rt] >> registers[rs];
        //printf("srlv $%d, $%d, $%d", rd, rt, rs);
    } else if (funct == 7) {
        registers[rd] = (signed int)registers[rt] >> registers[rs];
        //printf("srav $%d, $%d, $%d", rd, rt ,rs);
    /*} else if (funct == 8) {
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
        printf("mflo $%d", rd);
    } else if (funct == 19){
        printf("mtlo $%d", rs);
    } else if (funct == 24){
        printf("mult $%d, $%d", rs, rt);
    } else if (funct == 25){
        printf("multu $%d, $%d", rs, rt);
    } else if (funct == 26){
        printf("div $%d, $%d", rs, rt);
    } else if (funct == 27){
        printf("divu $%d, $%d", rs, rt); */
    } else if (funct == 32){
        registers[rd] = registers[rs] + registers[rt];
        //printf("add $%d, $%d, $%d", rd, rs, rt);
    } else if (funct == 33){
        registers[rd] = registers[rs] + registers[rt];
        //printf("addu $%d, $%d, $%d", rd, rs, rt);
    } else if (funct == 34){
        registers[rd] = registers[rs] - registers[rt];
        //printf("sub $%d, $%d, $%d", rd, rs, rt);
    } else if (funct == 35){
        registers[rd] = registers[rs] - registers[rt];
        //printf("subu $%d, $%d, $%d", rd, rs, rt);
    } else if (funct == 36){
        registers[rd] = registers[rs] & registers[rt];
        //printf("and $%d, $%d, $%d", rd, rs, rt);
    } else if (funct == 37){
        registers[rd] = registers[rs] | registers[rt];
        //printf("or $%d, $%d, $%d", rd, rs, rt);
    } else if (funct == 38){
        registers[rd] = registers[rs] ^ registers[rt];
        //printf("xor $%d, $%d, $%d", rd, rs, rt);
    } else if (funct == 39){
        registers[rd] = ~(registers[rs] ^ registers[rt]);
        //printf("nor $%d, $%d, $%d", rd, rs, rt);
    } else if (funct == 42){
        registers[rd] = (signed int)registers[rs] < (signed int)registers[rt];
        //printf("slt $%d, $%d, $%d", rd, rs, rt);
    } else if (funct == 43){
        registers[rd] = registers[rs] + registers[rt];
        //printf("sltu $%d, $%d, $%d", rd, rs, rt);
    } else if (funct == 0) { //sll
        registers[rd] = registers[rt] << shamt;
        //printf("sll $%d, $%d, %d", rd, rt, shamt);
    }
    else
    {       printf("unknown instruction");
    }
}


void Icommand(uch rs, uch rt, short immediate, uch opcode){

    if (opcode == 8) {
        registers[rt] = registers[rs] + immediate;
        //printf("addi $%d, $%d, %d", rt, rs, immediate);
    } else if (opcode == 9) {
        registers[rt] = registers[rs] + (unsigned int)immediate;
        //printf("addiu $%d, $%d, %d", rt, rs, immediate);
    } else if (opcode == 12) {
        registers[rt] = registers[rs] & immediate;
        //printf("andi $%d, $%d, %d", rt, rs, immediate);
    /*} else if (opcode == 4 ) {
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
        printf("lhu $%d, %d($%d)", rt, immediate, rs);*/
    } else if (opcode == 15) {
        registers[rt] = immediate << 16;
        //printf("lui $%d, %d", rt, immediate);
    //} else if (opcode == 35) {
    //    printf("lw $%d, %d($%d)", rt, immediate, rs);
    } else if (opcode == 13) {
        registers[rt] = registers[rs] | immediate;
        //printf("ori $%d, $%d, %d", rt, rs, immediate);
    //} else if (opcode == 40) {
    //    printf("sb $%d, %d($%d)", rt, immediate, rs);
    } else if (opcode == 10) {
        registers[rt] = registers[rt] <immediate;
        //printf("slti $%d, $%d, %d", rt, rs, immediate);
    } else if (opcode == 11) {
        registers[rt] = registers[rt] <(unsigned int)immediate;
        //printf("sltiu $%d, $%d, %d", rt, rs, immediate);
    //} else if (opcode == 41) {
    //    printf("sh $%d, %d($%d)", rt, immediate, rs);
    //} else if (opcode == 43) {
    //    printf("sw $%d, %d($%d)", rt, immediate, rs);
    } else if (opcode == 14) {
        registers[rt] = registers[rs] ^ immediate;
        //printf("xori $%d, $%d, %d", rt, rs, immediate);
    }
    else
        printf("unknown instruction");
}



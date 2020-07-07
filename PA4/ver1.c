#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>


typedef struct _data{
	unsigned int tag;
	unsigned int address;
	unsigned int offset;
}Data;

int bit_slice(unsigned int data, int left, int right);


int main (int argc, char * argv[]){

	int  cache[1024];

        FILE * fp1;
        char txtname[20];
        char buffer[20];
	char line[20];
	unsigned int number;
	Data data;
	int mask;
	int miss = 0;
	int write = 0;

        if(argc != 3)
        {
                printf("Error!\n");
                return 0;
        }
        strcpy(txtname, "trace");

        strcat(txtname, argv[2]);
        strcat(txtname, ".txt");

//      printf("%s", txtname);

        if((fp1 = fopen(txtname, "rt")) == NULL){
                fputs("파일 열기 에러!", stderr);
                exit(1);
        }
        else { 
		if(strncmp(argv[1], "0", 1) == 0)
                {
                        int cache[1024];
			for(int i = 0; i<1024; i++)
			{
				cache[i] = -1;
			}


                	while (fgets(buffer, sizeof(buffer), fp1) != NULL) {
                printf("%s", buffer);
		
				strcpy(line, buffer);
				char * ptr = line;
				number = strtol(ptr+2, NULL, 16);
				//printf("%08d\n", number);

					
				for (int i = 31; i >= 0; i--) { 
					mask = 1 << i; 
					printf("%d", number & mask ? 1 : 0); 
					if (i % 8 == 0) 
						printf(" "); 
				}
				printf("\n");

				data.tag = bit_slice(number, 14, 31);
				data.address = bit_slice(number, 4, 13);
				data.offset = bit_slice(number, 0, 3);

				if(cache[data.address] != data.tag)
					miss++;
				cache[data.address] = data.tag;

				if(buffer[0] == 'S')
					write++;	



				 for (int i = 31; i >= 0; i--) {
                                        mask = 1 << i; 
                                       printf("%d", data.offset & mask ? 1 : 0);
                                        if (i % 8 == 0) 
                                                printf(" ");
                                }
                                printf("\n");

				printf("%d %d %d\n", data.tag, data.address, data.offset);							
			
			}
		} else if(strncmp(argv[1], "1", 1) == 0)
		{

		}

	printf("%d %d\n", miss, write);
		

	}
		

/*
            char *ptr = input + 5;
            if ((fp1 = fopen(ptr, "rb")) == NULL) {
                fputs("파일 열기 에러!", stderr);
                exit(1);
            } else {
                while (read = fread(&data, sizeof(int), 1, fp1) != 0) { // read data

                    pdata = htonl(data);

                    opcode = bit_slice(pdata, 26, 31);

                    printf("inst %d:  %08x ", inst_num++, pdata);

                    if (opcode == 0) { //r
		}
		printf("\n");
                }
                fclose(fp1);
                printf("\n");
            }

        } else {
            printf("unexpected input\n"); //unexpected
            return -1;
        }
    }
	*/

    return 0;

}

int bit_slice(unsigned int data, int left, int right) {

    unsigned int range;
    unsigned int val;

    if (right == 31) {
        range = ~0;
    } else {
        range = (1 << (right - left+1)) - 1;
    }
    val = range & (data >> left);
    return val;
}

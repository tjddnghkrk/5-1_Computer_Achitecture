#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>


typedef struct _data{
	unsigned int tag;
	unsigned int address;
	unsigned int offset;
}Data;

typedef struct _way{
	int tag[2];
	int dirty[2];
	int next;
}way;

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
//                printf("%s", buffer);
		
				strcpy(line, buffer);
				char * ptr = line;
				number = strtol(ptr+2, NULL, 16);
				//printf("%08d\n", number);

					
//				for (int i = 31; i >= 0; i--) { 
//					mask = 1 << i; 
//					printf("%d", number & mask ? 1 : 0); 
//					if (i % 8 == 0) 
//						printf(" "); 
//				}
//				printf("\n");

				data.tag = bit_slice(number, 14, 31);
				data.address = bit_slice(number, 4, 13);
				data.offset = bit_slice(number, 0, 3);

				if(cache[data.address] != data.tag)
					miss++;
				if(buffer[0] == 'L') 
					cache[data.address] = data.tag;

				if(buffer[0] == 'S')
					write++;	



//				 for (int i = 31; i >= 0; i--) {
//                                        mask = 1 << i; 
//                                       printf("%d", data.offset & mask ? 1 : 0);
//                                        if (i % 8 == 0) 
//                                                printf(" ");
//                                }
//                              printf("\n");

//				printf("%d %d %d\n", data.tag, data.address, data.offset);							
			
			}
		} else if(strncmp(argv[1], "1", 1) == 0)
		{


			way arr[2048];
			
			for(int i = 0; i<2048; i++)
                        {
                                arr[i].tag[0] = -1;
				arr[i].tag[1] = -1;
				arr[i].dirty[0] = 0;
				arr[i].dirty[1] = 0;
                        }

                        while (fgets(buffer, sizeof(buffer), fp1) != NULL) {
//                printf("%s", buffer);

                                strcpy(line, buffer);
                                char * ptr = line;
                                number = strtol(ptr+2, NULL, 16);
                                //printf("%08d\n", number);


//                              for (int i = 31; i >= 0; i--) { 
//                                      mask = 1 << i; 
//                                      printf("%d", number & mask ? 1 : 0); 
//                                      if (i % 8 == 0) 
//                                              printf(" "); 
//                              }
//                              printf("\n");
				

				

                                data.tag = bit_slice(number, 16, 31);
                                data.address = bit_slice(number, 6, 15);
                                data.offset = bit_slice(number, 0, 5);

//				printf("%d %d %d\n", data.tag, data.address, data.offset);  

				if(arr[data.address].tag[0] != data.tag  && arr[data.address].tag[1] != data.tag)
				{
				//	printf("hi");
					arr[data.address].tag[arr[data.address].next] = data.tag;
					if(arr[data.address].dirty[arr[data.address].next] == 1){
						write++;
					}
					if(buffer[0] == 'S')
					{
						arr[data.address].dirty[arr[data.address].next] = 1;
					}
					if(buffer[0] == 'L')
					{
						arr[data.address].dirty[arr[data.address].next] = 0;
					} 
					if(arr[data.address].next == 0)
						arr[data.address].next = 1;
				        if(arr[data.address].next == 1)
                                                arr[data.address].next = 0;
					
					miss++;
				}
				else if(arr[data.address].tag[0] == data.tag)
					{
					if(buffer[0] == 'S')
						arr[data.address].dirty[0] = 1;
					arr[data.address].next = 1;
					}
				else if(arr[data.address].tag[1] == data.tag){
					if(buffer[0] == 'S')
                                                arr[data.address].dirty[1] = 1;
					arr[data.address].next = 0;
					}

			}
		}

	printf("%d %d\n", miss, write);
		

	}
		

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

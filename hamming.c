#include<stdio.h>
#include<string.h>
#include<math.h>
int main(int argc , char *argv[]) {
	
	int datasize; 
	int redundantbits = 2;
	int *datacode;
	int xorResult=0; //XOR 
	int i,c;
	int counter=0;
	int totalBits,parityBitCalc,size;
	if(argc < 2){
		printf("usage:./hamming.out <data in binary to be encoded> <m=MSB first> \n");
		printf("e.g if 10 in decimal , in binary 1010  \n");
		printf("./hamming.out 1010 m  \n");
		printf("./hamming.out 0101   \n");
		return 0;
	}
	datasize = strlen(argv[1]) ;
	//calculate number of redundant bits required

	while(1){
		if(datasize <= (pow(2,redundantbits) -redundantbits -1))
			break;		
		else
			redundantbits++;
	}

	datacode = (int *)malloc((redundantbits + datasize )*sizeof(int));
	memset(datacode,0,sizeof(redundantbits + datasize));
	size = 2;
	if(argv[2] != NULL){
		if(strcmp(argv[2],"m")==0){
			//printf("MSB first \n");
			for(i=0;i<datasize/2;i++){
				c=argv[1][datasize -1 - i];
				argv[1][datasize-1-i]=argv[1][i];
				argv[1][i]=c;
			}
		}
	}
	//for(i=0;i<datasize;i++)
	//	printf("%c",argv[1][i]);
	//printf("\n");
	//return 0;
//Copy the databytes to datacode .. Keeping parity bits as 0 
	for(i=1;i<datasize+redundantbits;i++){
		if((i + 1) % size == 0){
			size*=2;	
			continue;
		}
		else{	 
			if((argv[1][counter] == '0') || (argv[1][counter] == '1'))
				datacode[i]=argv[1][counter++] - '0';
			else {
				printf("Invalid Input \n");
				exit(1);
			}
		}
	}

//	for(i=0;i<datasize+redundantbits;i++)
//		printf("%d",datacode[i]);
//	printf("\n");
//001000
	counter = redundantbits-1;
	while(counter >= 0){	
		totalBits = datasize + redundantbits; // Total Number of bits
		parityBitCalc = pow(2,counter); //parity bit location
		xorResult = 0;
		while(totalBits > parityBitCalc){
			if(totalBits & parityBitCalc)
			 	xorResult ^=datacode[totalBits-1];
			totalBits--;
		}
		if(xorResult == 0)
			datacode[parityBitCalc-1]=0;
		else
			
			datacode[parityBitCalc-1]=1;
		counter--;
	}			
//Output
	printf("Hamming output \n");
	if(argv[2]){
		if(strcmp(argv[2],"m")==0){
			for(i=datasize+redundantbits-1;i>0;i--)
				printf("%d",datacode[i]);
		
		}
		else
			goto LSB;
	}
	else
	{
LSB:		
		for(i=0;i<datasize+redundantbits;i++)
			printf("%d",datacode[i]);
	}
	printf("\n");
	free(datacode);
}

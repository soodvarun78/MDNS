#include<stdio.h>
#include<string.h>
#include<math.h>
int main(int argc , char *argv[]) {
	
	int datasize; 
	int redundantbits = 1;
	int *datacode;
	int i;
	int counter=0;
	int power,bitVal,paritybit;
	int checkresult=0;
	if(argc != 2){
		printf("usage:./hamming_c.out <data in binary to be encoded> \n");
		return 0;
	}
	datasize = strlen(argv[1]) ;
	//calculate number of redundant bits required

	while(1){ 
		//if(datasize > (pow(2,redundantbits+1) -(redundantbits+1) -1))
		if(datasize < (pow(2,redundantbits) ))
			break;
		else
			redundantbits++;
	}

	datacode = (int *)malloc((datasize )*sizeof(int));
	memset(datacode,0,sizeof(datasize));
//Copy the databytes to datacode .. Keeping parity bits as 0 
	for(i=0;i<datasize;i++){
			if((argv[1][i] == '0') || (argv[1][i] == '1'))
				datacode[i]=argv[1][i] - '0';
			else {
				printf("Invalid Input \n");
				exit(1);
			}
		}
	

//	for(i=0;i<datasize+redundantbits;i++)
//		printf("%d",datacode[i]);
//	printf("\n");
	paritybit = 1;
	power = 0;
	counter = 1;
	//010101100011
	printf("redundant %d \n",redundantbits);
	while(counter <= redundantbits){	
		bitVal=0;
		paritybit = pow(2,power);
		for(i=0;i<datasize;i++){
			if((i+1)  & paritybit )
				bitVal ^= datacode[i];
		}
		if(bitVal == 0)
			printf("parity %d  is correct \n",paritybit);
		else{
			printf("parity %d is incorrect \n",paritybit);
			checkresult |=paritybit;
		}
		counter++;
		power++;	
	}			
//Output
	if(checkresult != 0)	
		printf("bit Wrong error in %d \n",checkresult);
	printf("\n");
	free(datacode);
}

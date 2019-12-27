#include <stdio.h>
#include <netpacket/packet.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <linux/sockios.h>

#include <linux/if_ether.h>
#include <linux/wireless.h>
#include <string.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include<errno.h>

#define DEVICE_NAME "wlp3s0"

struct mdns_packet_q {
	unsigned short tID;
	unsigned short Flags;
	unsigned short nQS;
	unsigned short nAS;
	unsigned short nATRR;
	unsigned short nAURR;
	unsigned char fqdn[256];
	unsigned short QType; 
	unsigned short QClass; 
};

struct mdns_packet_q qPacket;

void create_mdns_data_new(unsigned char *data,size_t *datalen,unsigned short flags,unsigned char *pFQDN,unsigned short QType){
	int len=0;
	unsigned short temp;
	unsigned int i=0,j=0,offset,count=0,k=0;
	unsigned char label[256];
//Transaction
	temp = htons((unsigned short)0x0000);
	memcpy(&qPacket.tID,&temp,2);
	len +=2;

//Flag
	temp = htons((unsigned short)flags);
	memcpy(&qPacket.Flags,&temp,2);
	len +=2;

//Questions
	temp = htons((unsigned short)1);
	memcpy(&qPacket.nQS,&temp,2);
	len +=2;

//Answers
	temp = htons((unsigned short)0x0000);
	memcpy(&qPacket.nAS,&temp,2);
	len +=2;

//Authority R Records
	temp = htons((unsigned short)0x0000);
	memcpy(&qPacket.nATRR,&temp,2);
	len +=2;

//Additional Records
	temp = htons((unsigned short)0x0000);
	memcpy(&qPacket.nAURR,&temp,2);
	len +=2;
	
	memcpy(data,&qPacket,len);
//FQDN
	offset =0;	
	printf("%ld %ld \n",sizeof(pFQDN),sizeof(pFQDN[0]));
	while(pFQDN[i] != '\0'){
		if(pFQDN[i] != '.'){
			count++;
			label[k]=pFQDN[i];
			k++;
		}
		else {
			qPacket.fqdn[offset++] = count;
			memcpy(&qPacket.fqdn[offset],label,count);
			k = 0;
			offset +=count;
			count = 0;
		}
			i++;
	}
	qPacket.fqdn[offset++]=0;
	memcpy(data +len,qPacket.fqdn,offset);
	len +=offset;
	offset = 0;
	
	temp = htons((unsigned short)QType);
	memcpy(&qPacket.QType,&temp,2);
	memcpy(data +len,&qPacket.QType,2);
	len +=2;

	temp = htons((unsigned short)0x0001);
	memcpy(&qPacket.QClass,&temp,2);
	memcpy(data +len,&qPacket.QClass,2);
	len +=2;
	
	*datalen = len;
}

int main(int argc , char  *argv[]) {
	int i;
	int len;
	struct sockaddr_in Socket_Addr;
	int retval;
	int optval=1;
	int optlen = 4;
	struct msghdr msg;
	struct iovec msg_iovec;
	unsigned char fqdn [100];
	unsigned short qType = 0x0001;

	struct sockaddr_in inp;

	int Socket = socket(AF_INET, SOCK_DGRAM,0 );
	
	if(argc < 2){
		printf("Usage: argv[0] <fqdn> <qType>");
		printf("qtype: A or PTR");
		return ;
	}
	
	if(argv[1] != NULL )
		strncpy(fqdn,argv[1],100); 
	if(argv[2] != NULL) {
		if(!strcmp(argv[2],"A"))
			qType = 0x0001;
		else if (!strcmp(argv[2],"PTR"))
			qType = 0x000C;
		else {
			printf("invalid qType:Default A ");
			qType = 0x0001;
		}	
	} 

	if (Socket < 0)
	{
    		perror("socket() error");
    		return -1;
	}


	memset(&Socket_Addr,0,sizeof(Socket_Addr));
	memset(&inp,0,sizeof(inp));
	// bind the socket to the interface
	Socket_Addr.sin_family = AF_INET;
	Socket_Addr.sin_port = htons(5111);

	Socket_Addr.sin_addr.s_addr = INADDR_ANY;

	retval = setsockopt(Socket,SOL_SOCKET,SO_REUSEADDR,&optval,(socklen_t )optlen);
	printf("retval = %d \n",retval);

	retval =  bind(Socket, (struct sockaddr *)&Socket_Addr, sizeof(Socket_Addr)); 
	printf("retval = %d \n",retval);

	msg_iovec.iov_base = malloc(400*sizeof(char));
	msg_iovec.iov_len = 400;

	create_mdns_data_new(msg_iovec.iov_base,&msg_iovec.iov_len,0x0,fqdn,qType);
	inp.sin_family = AF_INET;
        inp.sin_port = htons(5353);
	
	retval = inet_aton("224.0.0.251",&inp.sin_addr);
	if(retval == 0)
		printf("error \n");
	memset(&msg,0,sizeof(msg));
	msg.msg_name = &inp;
	msg.msg_namelen = sizeof(inp);
	msg.msg_iov =  	&msg_iovec;
	msg.msg_iovlen = 1;
	msg.msg_control = NULL;
 	msg.msg_controllen = 0;
	msg.msg_flags = 0;

	
	retval= sendmsg(Socket,&msg,0);
	printf("retval sendmsg =%d \n",retval);
	printf("errno %d \n",errno);
	free(msg_iovec.iov_base);
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
 
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
 
#include <linux/can.h>
#include <linux/can/raw.h>


/*
struct can_frame 
 {
       canid_t can_id;              // 32 bit CAN_ID + EFF/RTR/ERR flags
         __u8    can_dlc;           // data length code: 0 .. 8
         __u8    data[8] __attribute__((aligned(8)));
};
 */

int main(void)
{
	int s;
	int nbytes;
	struct can_frame frame;
    
	struct sockaddr_can addr;
	struct ifreq ifr;
 
	char *ifname = "can0";
 
	if( (s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
    {
		perror("Error while opening socket");
		return -1;
	}
 
	strcpy(ifr.ifr_name, ifname);
	ioctl(s, SIOCGIFINDEX, &ifr);
 
	addr.can_family  = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex; 
 
	printf("%s at index %d\n", ifname, ifr.ifr_ifindex);
 
	if( bind( s, (struct sockaddr *)&addr, sizeof(addr) ) < 0 )
    {
		perror("Error in socket bind");
		return -2;
	}

	int i =0;
	int j =0;
    
	while(1)
	{
 
		frame.can_id  = 123;
		frame.can_dlc = 8;
        
		frame.data[0] = 0;
		frame.data[1] = 255;
		frame.data[2] = 127;
        frame.data[3] = 54;
        frame.data[4] = 1;
        frame.data[5] = 12;
        frame.data[6] = 255;
        frame.data[7] = 90;
        
        
		nbytes = write(s, &frame, sizeof(struct can_frame));
	}

	return 0;
}

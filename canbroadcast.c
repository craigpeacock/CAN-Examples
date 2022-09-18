
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/bcm.h>

#define NUM_FRAMES  1

// Refer to https://github.com/nebaruzdin/can-doc/blob/master/can-bcm.txt for more details
int main(int argc, char **argv)
{
	int s; 
	struct sockaddr_can addr;
	struct ifreq ifr;

	printf("CAN Sockets Demo\r\n");

	if ((s = socket(PF_CAN, SOCK_DGRAM, CAN_BCM)) < 0) {
		perror("Socket");
		return 1;
	}

	strcpy(ifr.ifr_name, "vcan0" );
	ioctl(s, SIOCGIFINDEX, &ifr);

	memset(&addr, 0, sizeof(addr));
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

    // Note a CAN_BCM socket must be connected instead of bound after socket
    //  creation (example without error checking):
	if (connect(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Connect");
		return 1;
	}

    // Create a struct to set up a sequence of four CAN frames
    struct {
            struct bcm_msg_head msg_head;
            struct can_frame frame[NUM_FRAMES];
    } mytxmsg;

    //   Typically, it is used with a structure in which the components can_id, flags
    //   (SETTIMER, STARTTIMER), count=0, ival2=100ms, nframes=1 are set and the user
    //   data in the can_frame structure is filled in accordingly. This variable is then
    //   transmitted with the write() system call on the socket to BCM.
    mytxmsg.msg_head.can_id         = 0x690;
    mytxmsg.msg_head.opcode         = TX_SETUP;
    mytxmsg.msg_head.flags          = SETTIMER | STARTTIMER | TX_CP_CAN_ID;
    mytxmsg.msg_head.nframes        = NUM_FRAMES;
    mytxmsg.msg_head.count          = 0U;
    mytxmsg.msg_head.ival1.tv_sec   = 0U;
    mytxmsg.msg_head.ival1.tv_usec  = 0U;
    mytxmsg.msg_head.ival2.tv_sec   = 1U;    // send every 1 sec
    mytxmsg.msg_head.ival2.tv_usec  = 0U;
    
    // When adding TX_CP_CAN_ID in the flags, no need to set can_id here
    mytxmsg.frame[0].can_dlc        = 8U;

    // Create dummy payload here
    const uint8_t dummydata[8] = {0xDE, 0xAD, 0xBE, 0xEF, 0xB1, 0x6B, 0x00, 0xB5};
    memcpy(mytxmsg.frame[0].data, dummydata, 8U);

    if (write(s, &mytxmsg, sizeof(mytxmsg)) != sizeof(mytxmsg)) {
        perror("Write");
        return 1;
    }

    // Wait for a character
    // During this period that the connection is still established,
    // the broadcast manager will periodically transmit based on the settings above.
    char ch = getchar();

	if (close(s) < 0) {
		perror("Close");
		return 1;
	}

	return 0;
}

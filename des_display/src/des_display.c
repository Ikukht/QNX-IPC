#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <string.h>
#include <errno.h>
#include <sys/neutrino.h>
#include "../../common/des.h"


int main(void) {

	int     rcvid;         // indicates who we should reply to
	int     chid;          // the channel ID
	Display_send_t request;

	// create a channel
	chid = ChannelCreate (0);
	if (chid == -1)
	{
		perror("failed to create the channel.");
		exit (EXIT_FAILURE);
	}
	printf("The display is running as process_id %d\n", getpid());
	// this is typical of a server:  it runs forever
	while (1) {
		// get the message, and print it
		rcvid = MsgReceive (chid, &request, sizeof (request),
				NULL);
		MsgReply (rcvid, 0, NULL, 0);

		switch (request.output_msg_index){
			case WS_MSG:
				printf("%s at %d \n",outMessage[request.output_msg_index], request.person.weight);

				break;
			case LS_MSG:
				printf("%s ID %d \n", outMessage[request.output_msg_index], request.person.id);

				break;
			case RS_MSG:
				printf("%s ID %d \n", outMessage[request.output_msg_index], request.person.id);

				break;
			default:
				printf("%s \n", outMessage[request.output_msg_index]);
				break;
		}
	}

	// destroy the channel when done
	ChannelDestroy(chid);
	return EXIT_SUCCESS;
}

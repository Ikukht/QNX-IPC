#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "../../common/des.h"
#include <process.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <string.h>

int display_pid;
int chid;
int coid_display;
int rcvid;

Input input_send;
Display_send_t display_send;
Person person;
Person input_person;

void left_scan (){

	printf(" IN left scan function \n");

	rcvid = MsgReceive (chid, &input_person, sizeof (input_person),
					NULL);
	MsgReply (rcvid, 0, NULL, 0);

	printf(" Received person id = %d \n", input_person.id);


	person.id = input_person.id;
	person.dir = INBOUND;
	display_send.person = person;
	display_send.output_msg_index = LS_MSG;

	if (MsgSend (coid_display, &display_send, sizeof(display_send) , 0, 0) == -1) {
		fprintf (stderr, "Error during MsgSend\n");
		perror (NULL);
		exit (EXIT_FAILURE);
	}
	person.state = LEFT_SCAN;

}

void right_scan (){
	printf(" IN RIGHT scan function \n");

	rcvid = MsgReceive (chid, &input_person, sizeof (input_person),
					NULL);
	MsgReply (rcvid, 0, NULL, 0);
	printf(" Received person id = %d \n", input_person.id);

	person.id = input_person.id;
	person.dir = OUTBOUND;
	display_send.person = person;
	display_send.output_msg_index = RS_MSG;

	if (MsgSend (coid_display, &display_send, sizeof(display_send) , 0, 0) == -1) {
		fprintf (stderr, "Error during MsgSend\n");
		perror (NULL);
		exit (EXIT_FAILURE);
	}

	person.state = RIGHT_SCAN;

}

void left_unlock (){

	display_send.output_msg_index = GLU_MSG;
	if (MsgSend (coid_display, &display_send, sizeof(display_send) , 0, 0) == -1) {
		fprintf (stderr, "Error during MsgSend\n");
		perror (NULL);
		exit (EXIT_FAILURE);
	}

	person.state = LEFT_UNLOCK;
}

void left_door_open (){

	display_send.output_msg_index = LO_MSG;
	if (MsgSend (coid_display, &display_send, sizeof(display_send) , 0, 0) == -1) {
		fprintf (stderr, "Error during MsgSend\n");
		perror (NULL);
		exit (EXIT_FAILURE);
	}

	person.state = LEFT_DOOR_OPEN;
}

void scale (){

	MsgReceive (chid, &input_person, sizeof (input_person),
					NULL);
	MsgReply (rcvid, 0, NULL, 0);
	printf(" Received person id = %d \n", input_person.weight);

	person.weight = input_person.weight;
	display_send.person = person;
	display_send.output_msg_index = WS_MSG;

	if (MsgSend (coid_display, &display_send, sizeof(display_send) , 0, 0) == -1) {
		fprintf (stderr, "Error during MsgSend\n");
		perror (NULL);
		exit (EXIT_FAILURE);
	}
	person.state = SCALE;
}

void left_door_close (){

	display_send.output_msg_index = LC_MSG;
	if (MsgSend (coid_display, &display_send, sizeof(display_send) , 0, 0) == -1) {
		fprintf (stderr, "Error during MsgSend\n");
		perror (NULL);
		exit (EXIT_FAILURE);
	}

	person.state = LEFT_DOOR_CLOSE;
}

void right_unlock (){

	display_send.output_msg_index = GRU_MSG;
	if (MsgSend (coid_display, &display_send, sizeof(display_send) , 0, 0) == -1) {
		fprintf (stderr, "Error during MsgSend\n");
		perror (NULL);
		exit (EXIT_FAILURE);
	}

	person.state = RIGHT_UNLOCK;
}

void right_door_open (){

	display_send.output_msg_index = RO_MSG;
	if (MsgSend (coid_display, &display_send, sizeof(display_send) , 0, 0) == -1) {
		fprintf (stderr, "Error during MsgSend\n");
		perror (NULL);
		exit (EXIT_FAILURE);
	}

	person.state = RIGHT_DOOR_OPEN;
}

void right_door_close (){

	display_send.output_msg_index = RC_MSG;
	if (MsgSend (coid_display, &display_send, sizeof(display_send) , 0, 0) == -1) {
		fprintf (stderr, "Error during MsgSend\n");
		perror (NULL);
		exit (EXIT_FAILURE);
	}
	person.state = RIGHT_DOOR_CLOSE;

}

void left_door_lock (){

	display_send.output_msg_index = GLL_MSG;
	if (MsgSend (coid_display, &display_send, sizeof(display_send) , 0, 0) == -1) {
		fprintf (stderr, "Error during MsgSend\n");
		perror (NULL);
		exit (EXIT_FAILURE);
	}

	person.state = LEFT_LOCK;

}

void right_door_lock (){

	display_send.output_msg_index = GRL_MSG;
	if (MsgSend (coid_display, &display_send, sizeof(display_send) , 0, 0) == -1) {
		fprintf (stderr, "Error during MsgSend\n");
		perror (NULL);
		exit (EXIT_FAILURE);
	}

	person.state = RIGHT_LOCK;

}

stateMachine_t stateMachine[] = {
		{ LEFT_SCAN, left_scan },
		{ RIGHT_SCAN, right_scan },
		{ LEFT_UNLOCK, left_unlock },
		{ LEFT_DOOR_OPEN, left_door_open },
		{ SCALE, scale },
		{ LEFT_DOOR_CLOSE, left_door_close },
		{ RIGHT_UNLOCK, right_unlock },
		{ RIGHT_DOOR_OPEN, right_door_open },
		{ RIGHT_DOOR_CLOSE, right_door_close },
		{ LEFT_LOCK, left_door_lock },
		{ RIGHT_LOCK, right_door_lock }
};

int main( int argc, char * argv[] ) {

	if(argc!=2){
		printf("Too few/many arguments \nEx ./des_controller 123456 1 + 1\n");
		return EXIT_FAILURE;
	}

	display_pid = atoi(argv[1]);
	// Connect to Display
	coid_display = ConnectAttach (ND_LOCAL_NODE, display_pid, 1, _NTO_SIDE_CHANNEL, 0);
	if (coid_display == -1) {
		fprintf (stderr, "Couldn't ConnectAttach to des_display \n");
		perror (NULL);
		exit (EXIT_FAILURE);
	}

	chid = ChannelCreate(0);
	if (chid == -1){
		perror("failed to create the channel for des_input");
		exit (EXIT_FAILURE);
	}

	printf("Server PID is %d, chid = %d\n", getpid(), chid);

	while (1) {

		// get the message, and print it
		rcvid = MsgReceive (chid, &input_send, sizeof (input_send),
				NULL);
		MsgReply (rcvid, 0, NULL, 0);

		printf(" received input type = %d \n", input_send);

		switch (input_send){
		case LS:
			stateMachine[LEFT_SCAN].func();
			break;
		case RS:
			stateMachine[RIGHT_SCAN].func();
			break;
		case GLU:
			stateMachine[LEFT_UNLOCK].func();
			break;
		case LO:
			stateMachine[LEFT_DOOR_OPEN].func();
			break;
		case WS:
			stateMachine[SCALE].func();
			break;
		case LC:
			stateMachine[LEFT_DOOR_CLOSE].func();
			break;
		case GLL:
			stateMachine[LEFT_LOCK].func();
			break;
		case GRU:
			stateMachine[RIGHT_UNLOCK].func();
			break;
		case RO:
			stateMachine[RIGHT_DOOR_OPEN].func();
			break;
		case RC:
			stateMachine[RIGHT_DOOR_CLOSE].func();
			break;
		case GRL:
			stateMachine[RIGHT_LOCK].func();
			break;
		case EXIT:

			break;
		}
		// now, prepare the reply.  We reuse "message"
//		MsgReply (rcvid, EOK, &response, sizeof (response));


	}

	// destroy the channel when done
	ChannelDestroy(chid);
	return EXIT_SUCCESS;
}

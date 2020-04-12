#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "../../common/des.h"
#include <process.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <string.h>


int main( int argc, char * argv[]) {

	pid_t server_id;
	int coid;
	char input[5];
	Input in;
	Person person;

	//client_send_t request;
	//server_response_t response;

	if(argc!=2){
		printf("Too few/many arguments \nEx ./calc_client 123456 AB\n");
		return EXIT_FAILURE;
	}

	server_id = atoi(argv[1]);

	printf("connecting to %d \n", server_id);

	coid = ConnectAttach (ND_LOCAL_NODE, server_id, 2, _NTO_SIDE_CHANNEL, 0);

	if (coid == -1) {
		fprintf (stderr, "Couldn't ConnectAttach\n");
		perror (NULL);
		exit (EXIT_FAILURE);
	}

	printf("Connected \n");

	while(1){
		scanf("%s", input);

		for(int i = 0; i < 12; i++){
			if( strcmp(inMessage[i], input) == 0){
				in = (Input)  i;
			}
		}
		switch(in){
			case LS:
			case RS:
				//send input number
				if (MsgSend (coid, &in, sizeof(in) , NULL, 0)) {
					fprintf (stderr, "Error during MsgSend\n");
					perror (NULL);
					exit (EXIT_FAILURE);
				}

				if (MsgSend (coid, &person, sizeof(person) , NULL, 0)) {
					fprintf (stderr, "Error during MsgSend\n");
					perror (NULL);
					exit (EXIT_FAILURE);
				}
				break;
			case WS:
				if (MsgSend (coid, &in, sizeof(in) , NULL, 0)) {
					fprintf (stderr, "Error during MsgSend\n");
					perror (NULL);
					exit (EXIT_FAILURE);
				}

				if (MsgSend (coid, &person, sizeof(person) , NULL, 0)) {
					fprintf (stderr, "Error during MsgSend\n");
					perror (NULL);
					exit (EXIT_FAILURE);
				}
				break;
			default:
				if (MsgSend (coid, &in, sizeof(in) , NULL, 0)) {
					fprintf (stderr, "Error during MsgSend\n");
					perror (NULL);
					exit (EXIT_FAILURE);
				}
				break;
		}

	}


	//Disconnect from the channel
	ConnectDetach(coid);
	return EXIT_SUCCESS;
}


/*
 * des.h
 *
 *  Created on: Nov 4, 2019
 *      Author: GG
 */

#ifndef DES_H_
#define DES_H_

#define NUM_STATES 8
#define NUM_OUTPUTS 12
#define NUM_INPUTS 12

typedef enum {
	LS = 0,
	RS = 1,
	GLU = 2,
	LO = 3,
	WS = 4,
	LC = 5,
	GLL = 6,
	GRU = 7,
	RO = 8,
	RC = 9,
	GRL = 10,
	EXIT = 11
} Input;

const char *inMessage[NUM_INPUTS] = {
		"ls",
		"rs",
		"glu",
		"lo",
		"ws",
		"lc",
		"gll",
		"gru",
		"ro",
		"rc",
		"grl",
		"exit"
};

typedef enum {
	LEFT_SCAN = 0,
	RIGHT_SCAN = 1,
	LEFT_UNLOCK = 2,
	LEFT_DOOR_OPEN = 3,
	SCALE = 4,
	LEFT_DOOR_CLOSE = 5,
	RIGHT_UNLOCK = 6,
	RIGHT_DOOR_OPEN = 7,
	RIGHT_DOOR_CLOSE = 8,
	LEFT_LOCK = 9,
	RIGHT_LOCK = 10
} State;

typedef enum {
	LS_MSG = 0,
	RS_MSG = 1,
	GLU_MSG = 2,
	LO_MSG = 3,
	WS_MSG = 4,
	LC_MSG = 5,
	GLL_MSG = 6,
	GRU_MSG = 7,
	RO_MSG = 8,
	RC_MSG = 9,
	GRL_MSG = 10,
	EXIT_MSG = 11
} Output;

const char *outMessage[NUM_OUTPUTS] = {
		"Person used left scanner",
		"Person used right scanner",
		"Guard unlocks left door.",
		"Left door open",
		"Person is weighted",
		"Left door closed",
		"Guard locks left door",
		"Guard unlocks right door",
		"Right door opened",
		"Right door closed",
		"Guard locks right door",
		"Exit"
};

typedef enum {
	INBOUND = 0,
	OUTBOUND = 1
} Direction;

/*
typedef struct {
	Input input;
	int value;
} Input_send_t;
*/

struct stateMachine{
	State state;
	void (* func) ();
}typedef stateMachine_t;

typedef struct {
	int id;
	int	weight;
	Direction dir;
	State state;
} Person;


typedef struct {
	Person person;
	Output output_msg_index;
} Display_send_t;

#endif /* DES_H_ */


#ifndef _TransportCode_H
#define _TransportCode_H

typedef enum {
  GETPRED = 0, // Get the prdecessor node
	FINDSUCC, // Get the successor node
	NOTIF, //  Notify other nodes
	JOIN, // Join request
	PUT, // Put request
	GET, // Get request
	REMOVEKEY, // Remove key request
	SETSUCC, // Set successor request
	SETPRED, // Set predecessor request
	CHECKPRED // Check the predecessor request
} transportCode;

#endif

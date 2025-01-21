#include "loadingBarC.h"
#include "symLink_48.sl"
#include "symLink_30.sl"
#include <pthread.h>
#include <unistd.h>


// GLOBAL VARS-------------------------------------------------------------------------------------
volatile int endLoadingBarBool = 1; //need to manually add this to header file
pthread_t loadingBarThreadId;
int progressLb = 0;

// Error Msg Constant Vars
char* F_LOADBAR = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

/*
// Struct to hold loadingBarC parameter while passing it to thead function as void pointer
// pthread_create can only pass a void* argument to the function. This struct stores that
// This struct is not its own header file cuz not a global struct. Used only for these sets of functions
// This is commented out so cuz it is automatically added to the header file (when using autoExec.sh) and having it here as well creates duplicate definitions. I still need it here sostruct is not its own header file cuz not a global struct. Used only for these sets of functions
typedef struct{
	int expectedDur;
	int removeTextBool;
	char* loadingBarMsg;
} lbParams;
*/

// This function adds the smoothing affect to the loading bar when the process finishes before the expected duration
void loadingBarCProcEndedEarly(){
	if (progressLb < 50){
                printf("\r%s", "Loading .......................................[56%]");
                progressLb=55;
                usleep(0.2*1000000);
	}
        if (progressLb < 75){
                printf("\r%s", "Loading .......................................[83%]");
                progressLb=85;
                usleep(0.15*1000000);
	}
	if (progressLb < 90){
                printf("\r%s", "Loading .......................................[97%]");
                progressLb=100;
                usleep(0.15*1000000);
	}
	if (progressLb <= 100){
                printf("\r%s", "Loading .......................................[99%]");
                usleep(0.15*1000000);
                printf("\r%s", "Loading .......................................[100%]\n");
                usleep(0.1*1000000);
	}
	
	fflush(stdout); //using fflush to ensure loading bar is printed
}

// This function ends the loading bar
void closeLoadingBarC(lbParams* lbParamsIn){
	// Vars
	char* FF = (char*) __func__;
	
	
	//end loadingBarCThread by setting this var to 0
	endLoadingBarBool = 0;
	loadingBarCProcEndedEarly(); //smooth out loading bar

	
	// If want to remove the loading bar and text
	if (lbParamsIn->removeTextBool == 1){
		printf("\r\033[F"); //move up a line since 100% print statement ends with '\n'
		printf("\r\033[K"); //clear line (loading bar)
		if (lbParamsIn->loadingBarMsg[0] != '\0'){ //if a loading bar message was provided
			printf("\r\033[F"); //move up a line
			printf("\r\033[K"); //clear line
		}
	}

	free(lbParamsIn); //free struct pointer
	
	if (pthread_join(loadingBarThreadId, NULL) != 0){
		fprintf(stderr, "\n===%s%s%s%s", F_LOADBAR, " - ", FF, "===	Error closing loading bar thread\n");
	}

	// Resetting global vars for future use
	progressLb = 0;
	endLoadingBarBool = 1;

}

// This function is the primary loading bar function
// This function is what runs in a thread, which it's contents are not in loadingBarC()
void* loadingBarCThread(void* lbParamsIn){ 
	// Vars -----------------------------------------------------------------------------------
	//Struct vars
	lbParams* params = (lbParams*) lbParamsIn;
	int expectedDur = params->expectedDur;
	char* msg = params->loadingBarMsg;
	//Other vars
	int timeoutDur = 25;
	double interval = 0.2;
	int currDur = 0;
	//Error message vars
	char* FF = (char*) __func__;
	//-----------------------------------------------------------------------------------------
	
	//print loading bar message if not empty
	if (msg[0] != '\0'){ printf("%s\n", params->loadingBarMsg);}

	while (endLoadingBarBool && currDur < timeoutDur){
                if (progressLb < 90){
			progressLb = progressLb + (int) ( (double) interval / (double) expectedDur * 100);
			printf("\rLoading .......................................[%d%s", progressLb, "%]");
		}
		currDur += interval;
		usleep(interval*1000000);
		fflush(stdout); //using fflush to ensure loading bar is printed
	}

	if (currDur >= timeoutDur){ //error message if takes too long
		fprintf(stderr, "\n===%s%s%s%s", F_LOADBAR, " - ", FF, "===	Thread went on for too long; aborting thread\n");
	}
	
	return NULL;
}

// This function prints a loading bar in a separate thread
lbParams* loadingBarC(int expectedDurIn, int removeTextBoolIn, char* loadingBarMsgIn){
	// Resetting global vars for future use
	progressLb = 0;
	endLoadingBarBool = 1;

	// Initializing lbParams struct
	lbParams* p = malloc(sizeof(lbParams));
	p->expectedDur = expectedDurIn;
	p->removeTextBool = removeTextBoolIn;
	p->loadingBarMsg = loadingBarMsgIn;
	
	// Create thread
	pthread_create(&loadingBarThreadId, NULL, loadingBarCThread, (void*) p);

	return p;
}

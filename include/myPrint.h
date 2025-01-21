#ifndef MYPRINT
#define MYPRINT

#include "symLink_44.sl"
#include "symLink_37.sl"
#include "symLink_45.sl"


// GLOBAL VARS-------------------------------------------------------------------------------------
extern char* strToPrint;
extern int* dataTypeArrMyPrint;
//-------------------------------------------------------------------------------------------------

//=================================================================================================
// DEFINES
//=================================================================================================


#define myPrintPrimary(...) \
	do { \
		int numArgs = COUNT_ARGS(__VA_ARGS__); \
		if (numArgs == 1){ twoArgsDefineMyFree(__VA_ARGS__, 2, ARGSWITHPADDING(__VA_ARGS__));} \
		else if (numArgs > 5){ puts("===myPrint===	Too many args provided; a maximum of 5 args are allowed");} \
		else{ \
			dataTypeArrMyPrint = makeArrayOfArgDataTypes(numArgs, ARGSWITHPADDING(__VA_ARGS__)); \
			myPrintMultipleArgs(0, numArgs, ARGSWITHPADDING(__VA_ARGS__)); \
		} \
	} while (0) \

#define myPrintCheckForNoArgs(...) \
	do { \
		int numArgs = COUNT_ARGS(__VA_ARGS__); \
		if (numArgs == 0){ myPrintNoArgs(0);} \
		else { __VA_OPT__( myPrintPrimary(__VA_ARGS__)); } \
	} while (0) \

#define myPrintNoLn(...) myPrintCheckForNoArgs(__VA_ARGS__) \


#define myPrint(...) \
	do { \
		if (COUNT_ARGS(__VA_ARGS__) > 1){ myPrintCheckForNoArgs(__VA_ARGS__); puts("");} \
		else{ myPrintNoLn(__VA_ARGS__);} \
	} while (0) \


#define myPerrorPrimary(...) \
	do { \
		int numArgs = COUNT_ARGS(__VA_ARGS__); \
		if (numArgs > 5){ puts("===myPrint - myPerror===	Too many args provided; a maximum of 5 args are allowed");} \
		else{ \
			dataTypeArrMyPrint = makeArrayOfArgDataTypes(numArgs, ARGSWITHPADDING(__VA_ARGS__)); \
			myPrintMultipleArgs(1, numArgs, __VA_ARGS__); \
		} \
	} while (0) \


#define myPerrorCheckForNoArgs(...) \
	do { \
		int numArgs = COUNT_ARGS(__VA_ARGS__); \
		if (numArgs == 0){ myPrintNoArgs(1);} \
		else { __VA_OPT__( myPerrorPrimary(__VA_ARGS__)); } \
	} while (0) \

#define myPerror(...) myPerrorCheckForNoArgs(__VA_ARGS__) \


#define debugmsgPrimary(...)  \
	do { \
		int numArgs = COUNT_ARGS(__VA_ARGS__); \
		if (numArgs > 5){ puts("===myPrint - myPerror===	Too many args provided; a maximum of 5 args are allowed");} \
		else{ \
			dataTypeArrMyPrint = makeArrayOfArgDataTypes(numArgs, ARGSWITHPADDING(__VA_ARGS__)); \
			myPrintMultipleArgs(2, numArgs, __VA_ARGS__); \
		} \
	} while (0) \


#define debugmsgCheckForNoArgs(...) \
	do { \
		int numArgs = COUNT_ARGS(__VA_ARGS__); \
		if (numArgs == 0){ myPrintNoArgs(2);} \
		else { __VA_OPT__( debugmsgPrimary(__VA_ARGS__)); } \
	} while (0) \

#define debugmsg(...) debugmsgCheckForNoArgs(__VA_ARGS__); \

//=================================================================================================

//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

void myPrintNoArgs(int stderrBool);
void myPrintMultipleArgs(int stderrBool, int numArgsIn, ...);
//=================================================================================================

#endif

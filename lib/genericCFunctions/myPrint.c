#include "symLink_44.sl"
#include "symLink_37.sl"
#include "symLink_45.sl"

// GLOBAL VARS-------------------------------------------------------------------------------------
char* strToPrint;
int* dataTypeArrMyPrint;

// Error Msg Constant Vars
char* F_MYPRINT = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------


//=================================================================================================
// CODE
//=================================================================================================

// This function prints statements if there are no arguments to any of the myPrint functions (myPrint, myPerror, debugmsg)
void myPrintNoArgs(int stderrBool){
	if (stderrBool == 0){ printf("myPrint no args\n"); return;}
	else if (stderrBool == 1){ fprintf(stderr,"===NO FILE OR FUNCTION NAME===	generic error message\n"); return;}
	else if (stderrBool == 2){ fprintf(stderr,"===NO FILE OR FUNCTION - DEBUG===	generic debug message\n"); return;}
}

// This function takes a variable number of inputs and prints it regardless of data type
void myPrintMultipleArgs(int stderrBool, int numArgsIn, ...){
	va_list args;
	va_start(args, numArgsIn);

	for (int i = 0; i < numArgsIn; i++){
		if (stderrBool == 1 && i == 0){ fprintf(stderr,"===%s%s", va_arg(args, char*), " - ");}
		else if (stderrBool == 1 && i == 1){ fprintf(stderr,"%s%s", va_arg(args, char*), "===	");}
		else if (stderrBool == 2 && i == 0){ fprintf(stderr,"===%s%s", va_arg(args, char*), " - DEBUG===	");}

		else if (dataTypeArrMyPrint[i] == 0){
			if (stderrBool == 0){ printf("%d", va_arg(args, int));}
			else { fprintf(stderr, "%d", va_arg(args, int));}
		}
		else if (dataTypeArrMyPrint[i] == 1){
			if (stderrBool == 0){ printf("%f", va_arg(args, double));}
			else { fprintf(stderr, "%f", va_arg(args, double));}
		}
		else if (dataTypeArrMyPrint[i] == 2){
			//even though data type of 1 represents a double, it is converted to float when using variadric functions
			if (stderrBool == 0){ printf("%f", va_arg(args, double));}
			else { fprintf(stderr, "%f", va_arg(args, double));}
		}
		else if (dataTypeArrMyPrint[i] == 3){
			//even though data type of 3 represents a char, it is converted to an int when using variadric functions. "%c" will convert it back to char in print statement
			if (stderrBool == 0){ printf("%c", va_arg(args, int));}
			else { fprintf(stderr, "%c", va_arg(args, int));}
		}
		else if (dataTypeArrMyPrint[i] == 4){
			if (stderrBool == 0){ printf("%s", va_arg(args, char*));}
			else { fprintf(stderr, "%s", va_arg(args, char*));}
		}
		else{ 
			char errorMsg[1024] = "===";
			strcat(errorMsg, F_MYPRINT);
			strcat(errorMsg, " - ");
			strcat(errorMsg, (char*) __func__);
			strcat(errorMsg, "===	Invalid data type. Accepted data types are: int, double, float, char, and string");

			perror(errorMsg);
		}

	}
	
	if (stderrBool == 1){ fprintf(stderr, "\n");}
	if (stderrBool == 2){ printf("\n");}

}
//=================================================================================================


//=================================================================================================
// BACKUP OF DEFINES
//=================================================================================================
/*
#define myPrintPrimary(...) \
        do { \
                int numArgs = COUNT_ARGS(__VA_ARGS__); \
                if (numArgs == 1){ twoArgsDefineMyFree(__VA_ARGS__, 2, ARGSWITHPADDING(__VA_ARGS__));} \
                else if (numArgs > 5){ puts("===myPrint===      Too many args provided; a maximum of 5 args are allowed");} \
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
                if (numArgs > 5){ puts("===myPrint - myPerror===        Too many args provided; a maximum of 5 args are allowed");} \
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
                if (numArgs > 5){ puts("===myPrint - myPerror===        Too many args provided; a maximum of 5 args are allowed");} \
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

*/
//=================================================================================================


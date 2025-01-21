#ifndef MYINSERT
#define MYINSERT

#include "symLink_43.sl"
#include "symLink_76.sl"


// GLOBAL VARS-------------------------------------------------------------------------------------
extern int globalMyInsertType;
extern char* returnStrMyInsert;
extern char** returnStrArrMyInsert;
//-------------------------------------------------------------------------------------------------

//=================================================================================================
// DEFINES
//=================================================================================================


#define insertDefine(_1, _2, _3) _Generic((_1), \
		char*: _Generic((_2), \
			char: insertChar, \
			int: insertIntToStr, \
			char*: insertStr , \
			default: badDataTypeStrPos2), \
		char**: _Generic((_2), \
			char*: insertStrToStrArr, \
			char**: insertStrArrToStrArr, \
			default: badDataTypeStrArrPos2), \
		default: badDataTypePos1 \
		)(_1, _2, _3);

#define myInsert(_1, _2, _3) ({ \
	void* returnArr; \
	do { \
		insertDefine(_1, _2, _3); \
		if (globalMyInsertType == 0){ returnArr = (char*) returnStrMyInsert;} \
		else if (globalMyInsertType == 1){ returnArr = (char**) returnStrArrMyInsert;}\
	} while(0); \
	returnArr; \

#define myAppend(_1, _2) insertDefine(_1, _2, getArrSize(_1)) \

//=================================================================================================

//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

char* insertStr(char* strIn, char* strToAdd, int pos);
char* insertIntToStr(char* strIn, int intToAdd, int pos);
char* insertChar(char* strIn, char charToAdd, int pos);
char** insertStrToStrArr(char** strArrIn, char* strToAdd, int pos);
char** insertStrArrToStrArr(char** strArrIn, char** strArrToAdd, int pos);
void badDataTypeStrPos2(...);
void badDataTypeStrArrPos2(...);
void badDataTypePos1(...);
//=================================================================================================

#endif

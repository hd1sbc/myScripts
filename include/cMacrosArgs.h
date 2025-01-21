#ifndef CMARCROSARGS
#define CMACROSARGS

#ifndef FIRST
#	define FIRST(A, ...) A
#endif

#ifndef SECOND
#	define SECOND(A, B, ...) B
#endif

#ifndef THIRD
#	define THIRD(A, B, C, ...) C
#endif

#ifndef FOURTH
#	define FOURTH(A, B, C, D, ...) D
#endif

#ifndef FIFTH
#	define FIFTH(A, B, C, D, E, ...) E
#endif

#ifndef ARGSWITHPADDINGHELPER
#	define ARGSWITHPADDINGHELPER(...) __VA_ARGS__, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
#endif

#ifndef ARGSWITHPADDING
#	define ARGSWITHPADDING(...) __VA_OPT__(ARGSWITHPADDINGHELPER(__VA_ARGS__))
#endif

#ifndef NO_ARGS_HELPER
#	define NO_ARGS_HELPER(...) ({ \
        int isEmpty = 0; \
        do { \
                int numArgs = COUNT_ARGS_HELPER1(__VA_ARGS__); \
                if (numArgs == 1){ isEmpty = 1;} \
        } while(0); \
        isEmpty; \
        }) \

#endif

#ifndef NO_ARGS
#	define NO_ARGS(...) NO_ARGS_HELPER("holderArg", ##__VA_ARGS__)
#endif

#ifndef COUNT_ARGS_HELPER1
#	define COUNT_ARGS_HELPER1(...) COUNT_ARGS_HELPER2(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#endif

#ifndef COUNT_ARGS_HELPER2
#	define COUNT_ARGS_HELPER2(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#endif

#ifndef COUNT_ARGS
#	define COUNT_ARGS(...) ({ \
		int numArgsReturn; \
		do { \
			if (NO_ARGS(__VA_ARGS__) == 1){ numArgsReturn = 0;} \
			else{ numArgsReturn = COUNT_ARGS_HELPER1(__VA_ARGS__);} \
		} while(0); \
		numArgsReturn; \
		}) \

#endif

#endif

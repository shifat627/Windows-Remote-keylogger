#include "str.h"

__declspec(dllexport) char StringCompare(char * src, char * dest){
	while( (*src!=0) && (*dest !=0) ){
		if(*src != *dest){
			return 0;
		}
		
		src++;
		dest++;
	}
	
	if( (*src!=0) && (*dest !=0) ){
		return 0;
	}
	
	return 1;
}

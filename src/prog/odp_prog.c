/*
Copyright (C) 2016 Simon Schmidt

This software is provided 'as-is', without any express or implied warranty.
This license and the above notice may not be removed or altered from any
source file and must appear in every source file that contains any substantial
part of this source file. You may use this software, or code extracted from it,
as desired.
*/
#include <odp.h>



int main(int argc, char *argv[]){
	odp_init_global(NULL, NULL);
	odp_init_local(ODP_THREAD_CONTROL);
	
	return 0;
}




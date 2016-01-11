#include <odp.h>



int main(int argc, char *argv[]){
	odp_init_global(NULL, NULL);
	odp_init_local(ODP_THREAD_CONTROL);
	
	return 0;
}

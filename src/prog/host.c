/*
 *
 * Copyright 2016 Simon Schmidt
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <stdio.h>
#include <odp.h>

#include <odp/helper/linux.h>
#include <odp/helper/eth.h>
#include <odp/helper/ip.h>
#include <ustack/netinput.h>

struct iofix {
	odp_pktio_t pktio;
};


#define PKT_LIST_SIZE 16
static void *iot_burst(void *arg) {
	struct iofix fix = *((struct iofix*)arg);
	odp_packet_t pkt_list [PKT_LIST_SIZE];
	int packets,i;
	for(;;){
		packets = odp_pktio_recv(fix.pktio, pkt_list, PKT_LIST_SIZE);
		//printf("packets: %d\n",packets);
		for(i=0;i<packets;++i){
			net_input(pkt_list[i]);
		}
	}
	return NULL;
}
#undef PKT_LIST_SIZE

odp_pool_t open_pool(const char* name,int num){
	odp_pool_param_t params;
	/* Create packet pool */
	odp_pool_param_init(&params);
	params.pkt.seg_len = (1<<14)-192;
	params.pkt.len     = (1<<14)-192;
	params.pkt.num     = num;
	params.type        = ODP_POOL_PACKET;
	return odp_pool_create(name, &params);
}

odp_pktio_t open_nic(const char* dev,odp_pool_t pool){
	odp_pktio_t pktio;
	//odp_queue_t inq_def;
	//odp_queue_param_t qparam;
	odp_pktio_param_t pktio_param;

	pktio_param.in_mode = ODP_PKTIN_MODE_RECV;
	pktio_param.out_mode = ODP_PKTOUT_MODE_SEND;
	pktio = odp_pktio_open(dev, pool, &pktio_param);
	odp_pktio_start(pktio);
	return pktio;
}


#define MAX_WORKERS 32
odph_linux_pthread_t thread_tbl[MAX_WORKERS];
int main(int argc, char *argv[]){
	if(argc<2)return 1;
	int i,n,cpu;
	odp_cpumask_t cpumask;
	odp_init_global(NULL, NULL);
	odp_init_local(ODP_THREAD_CONTROL);

	struct iofix fix;

	odp_pool_t pool = open_pool("pktin_pool",1024);
	//fix.pktio = open_nic("mytap",pool);
	fix.pktio = open_nic(argv[1],pool);

	n = odp_cpumask_default_worker(&cpumask, MAX_WORKERS);
	odph_linux_pthread_create(thread_tbl, &cpumask,
			iot_burst,&fix,ODP_THREAD_WORKER);

	odph_linux_pthread_join(thread_tbl, n);

	return 0;
}




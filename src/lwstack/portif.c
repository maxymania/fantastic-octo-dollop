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


#include "portif.h"
#include "pkt_info.h"

static int portif_filter(uint16be_t port,struct PortMatch* match) ODP_HOT_CODE;
proc_result_t portif_input(odp_packet_t pkt,void* args) ODP_HOT_CODE;

static int portif_filter(uint16be_t port,struct PortMatch* match){
	struct PortRange* range = match->range;
	uint16be_t* list = match->list;
	uint16_t i = 0,n = match->listLen;
	if(range){
		i = odp_be_to_cpu_16(port);
		do{
			if((i>=range->start) && (i<=range->end)) /* Found a range! */
				return 1;
		}while(range = range->next);
		return 0;
	}
	for(;i<n;++i){
		if(list[i]==port)return 1;
	}
	return 0;
}

proc_result_t portif_input(odp_packet_t pkt,void* args){
	proc_result_t result;
	struct pkt_info* info = odp_packet_user_area(pkt);
	struct PortAll* filter = args;
	result.packet_handle = pkt;
	result.next_action = NA_DROP;
	if(!odp_likely(odp_packet_has_tcp(pkt)||odp_packet_has_udp(pkt)||odp_packet_has_sctp(pkt) ))
		goto NO_PORT;
	
	if(portif_filter(info->src_port,filter->src)&&
		portif_filter(info->dst_port,filter->dst))
			result.next_action = NA_CONTINUE;
	
	return result;
	NO_PORT:
	result.next_action = NA_DROP;
	return result;
}



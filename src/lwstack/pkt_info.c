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

#include "pkt_info.h"
//#include <odp/helper/tcp.h>
#include <odp/helper/ip.h>

typedef struct{ uint64_t d[2]; } ipv6addr;
typedef struct{ uint16be_t src,dest; } ports;

proc_result_t pktinf_input(odp_packet_t pkt,void* args) ODP_HOT_CODE;


proc_result_t pktinf_input(odp_packet_t pkt,void* args){
	proc_result_t result;
	struct pkt_info *info = args;
	result.packet_handle = pkt;
	result.next_action = NA_CONTINUE;
	if(odp_packet_has_ipv4(pkt)){
		odph_ipv4hdr_t* hdr = odp_packet_l3_ptr(pkt,NULL);
		*((uint32be_t*)info->src_ip) = hdr->src_addr;
		*((uint32be_t*)info->dst_ip) = hdr->dst_addr;
	}else if(odp_packet_has_ipv6(pkt)){
		odph_ipv6hdr_t* hdr = odp_packet_l3_ptr(pkt,NULL);
		*((ipv6addr*)info->src_ip) = *((ipv6addr*)hdr->src_addr);
		*((ipv6addr*)info->dst_ip) = *((ipv6addr*)hdr->dst_addr);
	}
	if(odp_packet_has_tcp(pkt)||odp_packet_has_udp(pkt)||odp_packet_has_sctp(pkt)){
		ports p = *((ports*)odp_packet_l4_ptr(pkt,NULL));
		info->src_port = p.src;
		info->dst_port = p.dest;
	}
	return result;
}

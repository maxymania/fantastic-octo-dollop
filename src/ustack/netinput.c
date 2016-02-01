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

#include <ustack/netinput.h>
#include "arp.h"
#include <stdio.h>

void net_input(odp_packet_t pkt){
	if(odp_unlikely(odp_packet_has_error(pkt)))
		goto DISCARD;

	if(odp_packet_has_tcp(pkt)){
		printf("has_tcp_packet\n");
		goto DISCARD;
	}else if(odp_packet_has_udp(pkt)) {
		printf("has_udp_packet\n");
		goto DISCARD;
	}else if(odp_packet_has_arp(pkt)) {
		arp_input(pkt);
		return;
	}else if(odp_packet_has_icmp(pkt)) {
		printf("has_icmp_packet\n");
		goto DISCARD;
	}

	DISCARD:	
	odp_packet_free(pkt);
}




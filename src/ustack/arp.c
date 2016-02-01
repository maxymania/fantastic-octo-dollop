/*
 *
 * Copyright 2016 Simon Schmidt
 * Copyright 2011-2015 by Andrey Butok. FNET Community.
 * Copyright 2008-2010 by Andrey Butok. Freescale Semiconductor, Inc.
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

#include "arp.h"
#include "fnet_arp.h"

uint32be_t ipv4_address = 0;

void arp_input(odp_packet_t pkt){
	uint32_t size;
	odph_ethaddr_t eth_address;

	odp_pktio_t nic = odp_packet_input(pkt);
	if(odp_unlikely(nic==ODP_PKTIO_INVALID)) goto DISCARD;
	if(odp_unlikely(odp_pktio_mac_addr(nic,&eth_address,sizeof(eth_address))<0))
		goto DISCARD;

	fnet_arp_header_t *arp_hdr   = odp_packet_l3_ptr(pkt,&size);

	if( size < sizeof(arp_hdr)
		|| (odp_be_to_cpu_16(arp_hdr->hard_type) != FNET_ARP_HARD_TYPE)
		|| (arp_hdr->hard_size != FNET_ARP_HARD_SIZE)
		|| (odp_be_to_cpu_16(arp_hdr->prot_type) != ODPH_ETHTYPE_IPV4)
		|| (arp_hdr->prot_size != FNET_ARP_PROT_SIZE) )
		goto DISCARD;

	if(odp_unlikely(arp_hdr->sender_prot_addr == ipv4_address)) {
		goto DISCARD;
	}
	
	// TODO: add arp entry to a table.

	if ((odp_be_to_cpu_16(arp_hdr->op) == FNET_ARP_OP_REQUEST) && (arp_hdr->targer_prot_addr == ipv4_address))
	{
		odph_ethhdr_t *eth_hdr = odp_packet_l2_ptr(pkt,NULL);

		arp_hdr->op = odp_cpu_to_be_16(FNET_ARP_OP_REPLY); /* Opcode */

		arp_hdr->target_hard_addr = arp_hdr->sender_hard_addr;
		arp_hdr->sender_hard_addr = eth_address;

		uint32be_t myip = arp_hdr->targer_prot_addr;
		arp_hdr->targer_prot_addr = arp_hdr->sender_prot_addr;
		arp_hdr->sender_prot_addr = myip;

		eth_hdr->dst = eth_hdr->src;
		eth_hdr->src = eth_address;

		if(odp_unlikely(odp_pktio_send(nic,&pkt,1)<1))
			goto DISCARD;
		return;
	}
	
	DISCARD:
	odp_packet_free(pkt);
}


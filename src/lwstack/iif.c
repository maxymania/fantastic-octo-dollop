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


#include "iif.h"
#include "pkt_info.h"

struct ip6i {
	uint64_t p[2];
};

static int iif_v4(struct pkt_info* info,struct IPM *src,struct IPM *dest) ODP_HOT_CODE;
static int iif_v6(struct pkt_info* info,struct IPM *src,struct IPM *dest) ODP_HOT_CODE;
proc_result_t iif_input(odp_packet_t pkt,void* args) ODP_HOT_CODE;

static int iif_v4(struct pkt_info* info,struct IPM *src,struct IPM *dest) {
	struct IPM4 *f;
	uint32be_t ip;
	ip = *((uint32be_t*)info->src_ip);
	if(odp_likely(src!=NULL)){
		f = src->ip4;
		while(f){
			/* we assume a long linear search! */
			if(odp_unlikely((f->mask&ip)==f->addr))
				goto SRC_OK;
			f=f->next;
		}
		return 0;
	}
	SRC_OK:
	ip = *((uint32be_t*)info->dst_ip);
	if(odp_likely(dest!=NULL)){
		f = dest->ip4;
		while(f){
			/* we assume a long linear search! */
			if(odp_unlikely((f->mask&ip)==f->addr))
				return 1;
			f=f->next;
		}
		return 0;
	}
	return 1;
}

static int iif_v6(struct pkt_info* info,struct IPM *src,struct IPM *dest) {
	struct IPM6 *f;
	struct ip6i i;
	i = *((struct ip6i*)info->src_ip);
	if(odp_likely(src!=NULL)){
		f = src->ip6;
		while(f){
			/* we assume a long linear search! */
			if(odp_unlikely( ((f->mask[0]&i.p[0])==(f->addr[0]))&&((f->mask[1]&i.p[1])==(f->addr[1])) ))
				goto SRC_OK;
			f=f->next;
		}
		return 0;
	}
	SRC_OK:
	i = *((struct ip6i*)info->src_ip);
	if(odp_likely(dest!=NULL)){
		f = dest->ip6;
		while(f){
			/* we assume a long linear search! */
			if(odp_unlikely( ((f->mask[0]&i.p[0])==(f->addr[0]))&&((f->mask[1]&i.p[1])==(f->addr[1])) ))
				return 1;
			f=f->next;
		}
		return 0;
	}
	return 1;
}


proc_result_t iif_input(odp_packet_t pkt,void* args){
	proc_result_t result;
	struct IPAll* filter = args;
	struct pkt_info* info = odp_packet_user_area(pkt);
	result.packet_handle = pkt;
	if(odp_packet_has_ipv6(pkt))
		result.next_action = iif_v6(info,filter->src,filter->dest)?NA_CONTINUE:NA_DROP;
	else if(odp_likely(odp_packet_has_ipv4(pkt)))
		result.next_action = iif_v4(info,filter->src,filter->dest)?NA_CONTINUE:NA_DROP;
	else result.next_action = NA_DROP;
	return result;
}




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

#include "tfif.h"
#include <odp/helper/tcp.h>

//proc_result_t tfif_input(odp_packet_t pkt,void* args) ODP_HOT_CODE;

proc_result_t tfif_input(odp_packet_t pkt,void* args){
	proc_result_t result;
	struct TCPAll filter = *((struct TCPAll*)args);
	odph_tcphdr_t* hdr = odp_packet_l3_ptr(pkt,NULL);
	result.packet_handle = pkt;
	result.next_action = ((hdr->doffset_flags&filter.mask)==filter.data)?NA_CONTINUE:NA_DROP;
	return result;
}



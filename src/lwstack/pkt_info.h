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

#pragma once

#include "pipeline.h"

/*
 * pkt_info.h:
 * Packet-Info Datastructures.
 */

struct pkt_info {
	uint8_t src_ip[16];
	uint8_t dst_ip[16];
	uint16be_t src_port;
	uint16be_t dst_port;
	uint8_t src_mac[6];
	uint8_t dst_mac[6];
};

proc_result_t pktinf_input(odp_packet_t pkt,void* args);



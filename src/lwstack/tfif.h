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
 * tfif.h:
 * TCP Flags Input Filter
 */

typedef unsigned int bit;

/*
 * (odph_tcphdr_t*)->doffset_flags;
 */
struct TCPAll{
	uint16be_t mask;
	uint16be_t data;
}

/* args must be `struct TCPAll' */
proc_result_t tfif_input(odp_packet_t pkt,void* args);




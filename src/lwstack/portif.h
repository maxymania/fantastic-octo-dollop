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
 * portif.h:
 * Port Input Filter
 */

struct PortRange {
	uint16_t start,end;
	struct PortRange* next;
};

struct PortMatch {
	struct PortRange* range;
	uint16be_t* list;
	uint16_t listLen;
};
struct PortAll {
	struct PortMatch* src;
	struct PortMatch* dst;
};

/* args must be `struct PortMatch' */
proc_result_t portif_input(odp_packet_t pkt,void* args);




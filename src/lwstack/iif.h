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
 * iif.h:
 * IP Input Filter
 */

struct IPM4 {
	uint32be_t addr;
	uint32be_t mask;
	struct IPM4* next;
};

struct IPM6 {
	uint64_t addr[2];
	uint64_t mask[2];
	struct IPM6* next;
};

struct IPM {
	struct IPM4 *ip4;
	struct IPM6 *ip6;
};

struct IPAll {
	struct IPM *src;
	struct IPM *dest;
};

/* args must be `struct IPAll' */
proc_result_t iif_input(odp_packet_t pkt,void* args);




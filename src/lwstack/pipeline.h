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

#include <odp.h>

/*
 * pipeline.h:
 * Pipeline specific defs.
 */

typedef enum {
	NA_FORGET, /* Drop, but do not free! */
	NA_DROP, /* Drop packet and free it! */
	NA_CONTINUE, /* Continue to the next element in the chain! */
} next_action_t;

typedef struct{
	odp_packet_t  packet_handle;
	next_action_t next_action;
} proc_result_t;

typedef proc_result_t (*process_function_t)(odp_packet_t pkt,void* args);



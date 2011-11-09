/**
 * \addtogroup uip6
 * @{
 */
/*
 * Copyright (c) 2010, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 * $Id: rpl-of0.c,v 1.5 2010/06/14 12:44:37 nvt-se Exp $
 */
/**
 * \file
 *         An implementation of RPL's objective function 0.
 *
 * \author Joakim Eriksson <joakime@sics.se>, Nicolas Tsiftes <nvt@sics.se>
 */

#include "net/rpl/rpl.h"

#define DEBUG DEBUG_ANNOTATE
#include "net/uip-debug.h"

static void reset(void *);
static rpl_parent_t *best_parent(rpl_parent_t *, rpl_parent_t *);
static rpl_rank_t calculate_rank(rpl_parent_t *, rpl_rank_t);

rpl_of_t rpl_of0 = {
  reset,
  NULL,
  best_parent,
  calculate_rank,
  0
};

#define DEFAULT_RANK_INCREMENT  4
#define MINIMUM_RANK_INCREMENT  1
#define MAXIMUM_RANK_INCREMENT  16
#define MAXIMUM_RANK_STRETCH    4

static void
reset(void *dag)
{
  PRINTF("RPL: Resetting OF0\n");
}

static rpl_rank_t
calculate_rank(rpl_parent_t *p, rpl_rank_t base_rank)
{
  if(base_rank == 0) {
    if(p == NULL) {
      return INFINITE_RANK;
    }
    base_rank = p->rank;
  }

  if((rpl_rank_t)(base_rank + DEFAULT_RANK_INCREMENT) < base_rank) {
    PRINTF("RPL: OF0 rank %d incremented to infinite rank due to wrapping\n",
        base_rank);
    return INFINITE_RANK;
  }
  return base_rank + DEFAULT_RANK_INCREMENT;
}

static rpl_parent_t *
best_parent(rpl_parent_t *p1, rpl_parent_t *p2)
{
  PRINTF("RPL: Comparing parent ");
  PRINT6ADDR(&p1->addr);
  PRINTF(" (confidence %d, rank %d) with parent ",
        p1->local_confidence, p1->rank);
  PRINT6ADDR(&p2->addr);
  PRINTF(" (confidence %d, rank %d)\n",
        p2->local_confidence, p2->rank);

  if(p1->rank < p2->rank) {
    return p1;
  } else if(p2->rank < p1->rank) {
    return p2;
  }

  if(p1->local_confidence > p2->local_confidence) {
    return p1;
  } else if(p2->local_confidence > p1->local_confidence) {
    return p2;
  }

  return p2;
}

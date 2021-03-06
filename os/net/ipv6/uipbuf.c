/*
 * Copyright (c) 2017, RISE SICS
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
 * 3. The name of the authors may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 *
 */
#include "contiki.h"
#include "uip.h"

/* Get the next header given the buffer - start indicates that this is
   start of the IPv6 header - needs to be set to 0 when in an ext hdr */
uint8_t*
uipbuf_get_next_header(uint8_t *buffer, uint16_t size, uint8_t *protocol, uint8_t start)
{
  int ext_len = 0;
  struct uip_ip_hdr *ipbuf = (struct uip_ip_hdr *) buffer;
  struct uip_ext_hdr *ext = NULL;

  if(start) {
    /* protocol in the IP buffer */
    *protocol = ipbuf->proto;
    return buffer + UIP_IPH_LEN;
  } else {
    /* protocol in the Ext hdr */
    ext = (struct uip_ext_hdr *) buffer;
    *protocol = ext->next;
    /* This is just an ext header */
    ext_len = (ext->len << 3) + 8;
    return buffer + ext_len;
  }
}

/* Get the final header given the buffer - that is assumed to be at start
   of an IPv6 header */
uint8_t*
uipbuf_get_last_header(uint8_t *buffer, uint16_t size, uint8_t *protocol)
{
  uint8_t *nbuf;

  nbuf = uipbuf_get_next_header(buffer, size, protocol, 1);
  while(uip_is_proto_ext_hdr(*protocol)) {
    /* send in and move beyond the ext hdr */
    nbuf = uipbuf_get_next_header(nbuf, size - (nbuf - buffer), protocol, 0);
  }
  return nbuf;
}

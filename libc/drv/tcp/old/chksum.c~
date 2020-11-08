//
#include <stdio.h>
#include "chksum.h"

//
static WORD _chksum(WORD *sdata, WORD len)
{
  WORD acc;
  
  for(acc = 0; len > 1; len -= 2) {
    acc += *sdata;
    if(acc < *sdata) {
      /* Overflow, so we add the carry to acc (i.e., increase by
         one). */
      ++acc;
    }
    ++sdata;
  }

  /* add up any odd byte */
  if(len == 1) {
    acc += htons(((WORD)(*(u8_t *)sdata)) << 8);
    if(acc < htons(((WORD)(*(u8_t *)sdata)) << 8)) {
      ++acc;
    }
  }

  return acc;
}
/*-----------------------------------------------------------------------------------*/
WORD uip_ipchksum(void)
{
  return _chksum((WORD *)&uip_buf[UIP_LLH_LEN], 20);
}
/*-----------------------------------------------------------------------------------*/
WORD
uip_tcpchksum(PACKET *p)
{
	WORD hsum, sum;
	void *p;

	/* Compute the checksum of the TCP header. */
	// ((1))
	hsum = _chksum((WORD *)&uip_buf[20 + UIP_LLH_LEN], 20);

	/* Compute the checksum of the data in the TCP packet and add it to
	   the TCP header checksum. */
	// ((2))
	sum = _chksum((WORD *)uip_appdata,
		(WORD)(((((WORD)(BUF->len[0]) << 8) + BUF->len[1]) - 40)));
	if((sum += hsum) < hsum) {
		++sum;
	}
  
	// ((3))
	if((sum += BUF->srcipaddr[0]) < BUF->srcipaddr[0]) {
		++sum;
	}
	if((sum += BUF->srcipaddr[1]) < BUF->srcipaddr[1]) {
		++sum;
	}
  if((sum += BUF->destipaddr[0]) < BUF->destipaddr[0]) {
    ++sum;
  }
  if((sum += BUF->destipaddr[1]) < BUF->destipaddr[1]) {
    ++sum;
  }
  if((sum += (WORD)htons((WORD)IP_PROTO_TCP)) < (WORD)htons((WORD)IP_PROTO_TCP)) {
    ++sum;
  }

  hsum = htons(htons(p->ip.len)-20);
  
  if((sum += hsum) < hsum) {
    ++sum;
  }
  
  return sum;
}
/*-----------------------------------------------------------------------------------*/

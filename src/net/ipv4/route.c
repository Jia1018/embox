/**
 * \file route.c
 * \date 16.11.09
 * \author sikmir
 * \brief implementation of the IP router.
 */

#include "net/route.h"
#include "net/skbuff.h"
#include "net/arp.h"
#include "lib/inet/netinet/in.h"

#define RT_TABLE_SIZE 16

/**
 * NOTE: Linux route uses 3 structs for routing:
 *    + Forwarding Information Base (FIB)
 *    - routing cache (256 chains)
 *    - neibour table
 */
static struct rt_entry rt_table[RT_TABLE_SIZE];

int rt_add_route(struct net_device *dev, in_addr_t dst,
        		    in_addr_t mask, in_addr_t gw) {
	int i;
        for (i = 0; i < RT_TABLE_SIZE; i++) {
                if (!rt_table[i]._is_up) {
            		rt_table[i].dev        = dev;
            		rt_table[i].rt_dst     = dst;
            		rt_table[i].rt_mask    = mask;
            		rt_table[i].rt_gateway = gw;
                        rt_table[i]._is_up     = 1;
                        return 0;
                }
        }
        return -1;
}

int rt_del_route(struct net_device *dev, in_addr_t dst,
			    in_addr_t mask, in_addr_t gw) {
	int i;
	for(i = 0; i < RT_TABLE_SIZE; i++) {
	        if ((rt_table[i].rt_dst == dst || INADDR_ANY == dst) &&
	    	    (rt_table[i].rt_mask == mask || INADDR_ANY == mask) &&
	    	    (rt_table[i].rt_gateway == gw || INADDR_ANY == gw) ) {
	                rt_table[i]._is_up = 0;
	                return 0;
	        }
	}
}

int ip_route(sk_buff_t *skbuff) {
	int i;
	char buf[15];
	for(i = 0; i < RT_TABLE_SIZE; i++) {
		ipaddr_print(buf, skbuff->nh.iph->daddr);
		if( (inet_addr(buf) & rt_table[i].rt_mask) == rt_table[i].rt_dst) {
			skbuff->netdev = rt_table[i].dev;
			//TODO: fix addr format.
			//arp_resolve_addr(skbuff, rt_table[i].rt_gateway);
			return 0;
		}
	}
	return -1;
}

static int rt_iter;

struct rt_entry *rt_fib_get_first() {
	for(rt_iter = 0; rt_iter < RT_TABLE_SIZE; rt_iter++) {
    		if (1 == rt_table[rt_iter]._is_up) {
            		rt_iter++;
            		return &rt_table[rt_iter - 1];
    		}
        }
        return NULL;
}

struct rt_entry *rt_fib_get_next() {
	for(; rt_iter < RT_TABLE_SIZE; rt_iter++) {
    		if (1 == rt_table[rt_iter]._is_up) {
                	rt_iter++;
                        return &rt_table[rt_iter - 1];
                }
        }
        return NULL;
}

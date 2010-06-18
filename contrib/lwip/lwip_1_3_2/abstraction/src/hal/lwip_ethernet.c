/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/*
 * This file is a skeleton for developing Ethernet network interface
 * drivers for lwIP. Add code to the low_level functions and do a
 * search-and-replace for the word "ethernetif" to replace it with
 * something that better describes your network interface.
 */

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include <lwip/stats.h>
#include <lwip/snmp.h>
#include "netif/etharp.h"
#include "netif/ppp_oe.h"

/* hw abstraction layer */
#include "hal/lwip_ethernet.h"

/* Define those to better describe your network interface. */
#define IFNAME0 'e'
#define IFNAME1 'n'

static struct pbuf *low_level_input(struct netif *netif);
static err_t low_level_init(struct netif *netif);
static err_t low_level_output(struct netif *netif, struct pbuf *p);

/**
 * This function should be called by the program to service the network interface.
 * It calls the function low_level_rx() to do the actual input.
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
err_t ethernetif_service(struct netif *netif)
{
  err_t ret_code = 0;
  //struct ethernetif* dev = (struct ethernetif*)netif->state;

  LWIP_DEBUGF(NETIF_DEBUG | LWIP_DBG_TRACE, ("ethernetif_service()\n"));

  /* Call the device input routine, passing the LWIP input handler */
  ret_code = ethernetif_input(netif);

  LWIP_DEBUGF(NETIF_DEBUG | LWIP_DBG_TRACE, ("ethernetif_service() returns = %d\n",ret_code));

  return ret_code;
}

/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static err_t low_level_init(struct netif *netif)
{
	//struct ethernetif *ethernetif = netif->state;
	
	LWIP_DEBUGF(NETIF_DEBUG | LWIP_DBG_TRACE, ("low_level_init ( %#x)\n",netif));

	/* set MAC hardware address length */
	netif->hwaddr_len = ETHARP_HWADDR_LEN;

	/* set MAC hardware address */
	netif->hwaddr[0] = MY_DEFAULT_MAC_BYTE1;
	netif->hwaddr[1] = MY_DEFAULT_MAC_BYTE2;
	netif->hwaddr[2] = MY_DEFAULT_MAC_BYTE3;
	netif->hwaddr[3] = MY_DEFAULT_MAC_BYTE4;
	netif->hwaddr[4] = MY_DEFAULT_MAC_BYTE5;
	netif->hwaddr[5] = MY_DEFAULT_MAC_BYTE6;
	
	/* maximum transfer unit */
	netif->mtu = 1500;

	/* device capabilities */
	/* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
	netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;

	/* Do whatever else is needed to initialize interface. */  
	mydevice_init();
	
	return ERR_OK;
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t ethernetif_init(struct netif *netif)
{
  struct ethernetif *ethernetif;

  LWIP_ASSERT("netif != NULL", (netif != NULL));
  LWIP_DEBUGF(NETIF_DEBUG | LWIP_DBG_TRACE, ("ethernetif_init ( %#x)\n",netif));
    
	// Allocate memory for ethernetif
  ethernetif = mem_malloc(sizeof(struct ethernetif));
  if (ethernetif == NULL) {
    LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_init: out of memory\n"));
    return ERR_MEM;
  }

#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

  /*
   * Initialize the snmp variables and counters inside the struct netif.
   * The last argument should be replaced with your link speed, in units
   * of bits per second.
   */
  // NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, LINK_SPEED_OF_YOUR_NETIF_IN_BPS);

  netif->state = ethernetif;
  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;
  /* We directly use etharp_output() here to save a function call.
   * You can instead declare your own function an call etharp_output()
   * from it if you have to do some checks before sending (e.g. if link
   * is available...) */
  netif->output = etharp_output;
  netif->linkoutput = low_level_output;
  
  ethernetif->ethaddr = (struct eth_addr *)&(netif->hwaddr[0]);
  ethernetif->pkt_cnt = 0; 
  ethernetif->length = 0;
  
  /* initialize the hardware */
  low_level_init(netif);

  return ERR_OK;
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */
static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
	//struct ethernetif *ethernetif = netif->state;
	struct pbuf *q;
	u16_t length = p->tot_len;
	#ifdef LWIP_DEBUG
	//int i;
	#endif

	LWIP_DEBUGF(NETIF_DEBUG | LWIP_DBG_TRACE, ("low_level_output ( %#x)\n",p));

	mydevice_initiate_transfer();

	#if ETH_PAD_SIZE
	pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
	#endif

	for(q = p; q != NULL; q = q->next) {
		/* Send the data from the pbuf to the interface, one pbuf at a
		   time. The size of the data in each pbuf is kept in the ->len
		   variable. */
		// LWIP_DEBUGF(NETIF_DEBUG | LWIP_DBG_TRACE, ("\npayload: %x, len: %d\n", q->payload, q->len));
		// for(i=0; i<(q->len); i++)
		// {	
			// LWIP_DEBUGF(NETIF_DEBUG | LWIP_DBG_TRACE, ("q->payload[%d]:%x  ", i, ((u8_t*)q->payload)[i]));
		// }
		mydevice_write(q->payload, q->len);
	}
	
	LWIP_DEBUGF(NETIF_DEBUG | LWIP_DBG_TRACE, ("signal length: %d\n", length));
	mydevice_signal(length);
	
	#if ETH_PAD_SIZE
	pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
	#endif

	LINK_STATS_INC(link.xmit);
	
	LWIP_DEBUGF(NETIF_DEBUG | LWIP_DBG_TRACE, ("low_level_output () return OK\n"));

	return ERR_OK;
}

/**
 * This function should be called when a packet is ready to be transmittedd
 * to the interface. It uses the function low_level_output() that
 * should handle the actual transmission of bytes to the network
 * interface. 
 * @param netif the lwip network interface structure for this ethernetif.
 * @param p buffer to be transmitted.
 * @param ipaddr ip address.
 */
err_t ethernetif_output(struct netif *netif, struct pbuf *p, struct ip_addr *ipaddr)
{
	err_t ret;
	LWIP_DEBUGF(NETIF_DEBUG | LWIP_DBG_TRACE, ("ethernetif_output()\n"));

	ret = etharp_output(netif, p, ipaddr);

	if (p != NULL) 
	{
		low_level_output(netif, p);
	}

	LWIP_DEBUGF(NETIF_DEBUG | LWIP_DBG_TRACE, ("ethernetif_output() return OK\n"));

	return ERR_OK;
}

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf *low_level_input(struct netif *netif)
{
	struct ethernetif *ethernetif = netif->state;
	struct pbuf *p, *q;
	u16_t len;
	#ifdef LWIP_DEBUG
	int i;
	#endif

	/* Obtain the size of the packet and put it into the "len"
	variable. */
	
	mydevice_get_info(ethernetif);
    if(ethernetif->pkt_cnt==0)
		return (struct pbuf *)0;
		
	len = ethernetif->length;

	#if ETH_PAD_SIZE
	len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
	#endif

	/* We allocate a pbuf chain of pbufs from the pool. */
	p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
  
	if (p != NULL) 
	{
		#if ETH_PAD_SIZE
		pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
		#endif

		/* We iterate over the pbuf chain until we have read the entire
		* packet into the pbuf. */
		for(q = p; q != NULL; q = q->next) {
		/* Read enough bytes to fill this pbuf in the chain. The
		* available data in the pbuf is given by the q->len
		* variable.
		* This does not necessarily have to be a memcpy, you can also preallocate
		* pbufs for a DMA-enabled MAC and after receiving truncate it to the
		* actually received size. In this case, ensure the tot_len member of the
		* pbuf is the sum of the chained pbuf len members.
		*/
			mydevice_read(q->payload, q->len);
			LWIP_DEBUGF(NETIF_DEBUG | LWIP_DBG_TRACE, ("\npayload: %x, len: %d\n", q->payload, q->len));
			for(i=0; i<(q->len); i++)
			{	
			LWIP_DEBUGF(NETIF_DEBUG | LWIP_DBG_TRACE, ("q->payload[%d]:%x  ", i, ((u8_t*)q->payload)[i]));
			}
		}
		mydevice_ack();

		#if ETH_PAD_SIZE
		pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
		#endif

		LINK_STATS_INC(link.recv);
	} 
	else {
		mydevice_drop_packet();
		LINK_STATS_INC(link.memerr);
		LINK_STATS_INC(link.drop);
	}

  return p;  
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
err_t ethernetif_input(struct netif *netif)
{
  struct ethernetif *ethernetif;
  struct eth_hdr *ethhdr;
  struct pbuf *p;

  ethernetif = netif->state;

  /* move received packet into a new pbuf */
  p = low_level_input(netif);
  /* no packet could be read, silently ignore this */
  if (p == NULL) 
  {
	LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: p == NULL!\n"));
	return ERR_OK;
  }
  /* points to packet payload, which starts with an Ethernet header */
  ethhdr = p->payload;

  switch (htons(ethhdr->type)) {
  /* IP or ARP packet? */
  case ETHTYPE_IP:
  case ETHTYPE_ARP:
#if PPPOE_SUPPORT
  /* PPPoE packet? */
  case ETHTYPE_PPPOEDISC:
  case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
    /* full packet send to tcpip_thread to process */
    if (netif->input(p, netif)!=ERR_OK)
     { LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error!\n"));
       pbuf_free(p);
       p = NULL;
     }
    break;

  default:
	LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: type unknown!\n"));
    pbuf_free(p);
    p = NULL;
    break;
  }
  return ERR_OK;
}

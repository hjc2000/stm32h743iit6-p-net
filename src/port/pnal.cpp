/*********************************************************************
 *        _       _         _
 *  _ __ | |_  _ | |  __ _ | |__   ___
 * | '__|| __|(_)| | / _` || '_ \ / __|
 * | |   | |_  _ | || (_| || |_) |\__ \
 * |_|    \__|(_)|_| \__,_||_.__/ |___/
 *
 * www.rt-labs.com
 * Copyright 2021 rt-labs AB, Sweden.
 *
 * This software is dual-licensed under GPLv3 and a commercial
 * license. See the file LICENSE.md distributed with this software for
 * full license information.
 ********************************************************************/

#include "pnal.h"
#include "options.h"
#include "osal.h"
#include "osal_log.h"
#include <algorithm>
#include <bsp-interface/di/console.h>
#include <lwip/apps/snmp.h>
#include <lwip/netif.h>
#include <lwip/snmp.h>
#include <lwip/sys.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

int pnal_set_ip_suite(char const *interface_name,
					  pnal_ipaddr_t const *p_ipaddr,
					  pnal_ipaddr_t const *p_netmask,
					  pnal_ipaddr_t const *p_gw,
					  char const *hostname,
					  bool permanent)
{
	ip_addr_t ip_addr;
	ip_addr_t ip_mask;
	ip_addr_t ip_gw;

	ip_addr.addr = htonl(*p_ipaddr);
	ip_mask.addr = htonl(*p_netmask);
	ip_gw.addr = htonl(*p_gw);
	netif_set_addr(netif_default, &ip_addr, &ip_mask, &ip_gw);

	return 0;
}

int pnal_get_macaddress(char const *interface_name, pnal_ethaddr_t *mac_addr)
{
	memcpy(mac_addr, netif_default->hwaddr, sizeof(pnal_ethaddr_t));
	return 0;
}

pnal_ipaddr_t pnal_get_ip_address(char const *interface_name)
{
	return htonl(netif_default->ip_addr.addr);
}

pnal_ipaddr_t pnal_get_netmask(char const *interface_name)
{
	return htonl(netif_default->netmask.addr);
}

pnal_ipaddr_t pnal_get_gateway(char const *interface_name)
{
	/* TODO Read the actual default gateway */

	pnal_ipaddr_t ip;
	pnal_ipaddr_t gateway;

	ip = pnal_get_ip_address(interface_name);
	gateway = (ip & 0xFFFFFF00) | 0x00000001;

	return gateway;
}

int pnal_get_hostname(char *hostname)
{
	std::string name{"p-net"};
	std::copy(name.c_str(), name.c_str() + name.size() + 1, hostname);
	return 0;
}

int pnal_get_ip_suite(char const *interface_name,
					  pnal_ipaddr_t *p_ipaddr,
					  pnal_ipaddr_t *p_netmask,
					  pnal_ipaddr_t *p_gw,
					  char *hostname)
{
	int ret = -1;

	*p_ipaddr = pnal_get_ip_address(interface_name);
	*p_netmask = pnal_get_netmask(interface_name);
	*p_gw = pnal_get_gateway(interface_name);
	ret = pnal_get_hostname(hostname);

	return ret;
}

int pnal_get_port_statistics(char const *interface_name,
							 pnal_port_stats_t *port_stats)
{
	port_stats->if_in_octets = netif_default->mib2_counters.ifinoctets;
	port_stats->if_in_errors = netif_default->mib2_counters.ifinerrors;
	port_stats->if_in_discards = netif_default->mib2_counters.ifindiscards;
	port_stats->if_out_octets = netif_default->mib2_counters.ifoutoctets;
	port_stats->if_out_errors = netif_default->mib2_counters.ifouterrors;
	port_stats->if_out_discards = netif_default->mib2_counters.ifoutdiscards;

	return 0;
}

int pnal_get_interface_index(char const *interface_name)
{
	return 0;
}

int pnal_eth_get_status(char const *interface_name, pnal_eth_status_t *status)
{
	status->is_autonegotiation_supported = false;
	status->is_autonegotiation_enabled = false;
	status->autonegotiation_advertised_capabilities = 0;
	status->operational_mau_type = PNAL_ETH_MAU_COPPER_100BaseTX_FULL_DUPLEX;
	status->running = true;
	return 0;
}

int pnal_save_file(char const *fullpath,
				   void const *object_1,
				   size_t size_1,
				   void const *object_2,
				   size_t size_2)
{
	// TODO
	return -1;
}

void pnal_clear_file(char const *fullpath)
{
}

int pnal_load_file(char const *fullpath,
				   void *object_1,
				   size_t size_1,
				   void *object_2,
				   size_t size_2)
{
	// TODO
	return -1;
}

uint32_t pnal_get_system_uptime_10ms(void)
{
	uint32_t uptime = 0;
	MIB2_COPY_SYSUPTIME_TO(&uptime);
	return uptime;
}

pnal_buf_t *pnal_buf_alloc(uint16_t length)
{
	return pbuf_alloc(PBUF_RAW, length, PBUF_POOL);
}

void pnal_buf_free(pnal_buf_t *p)
{
	pbuf_free(p);
}

uint8_t pnal_buf_header(pnal_buf_t *p, int16_t header_size_increment)
{
	return pbuf_header(p, header_size_increment);
}

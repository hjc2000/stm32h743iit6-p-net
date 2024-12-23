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

#include "app_gsdml.h"
#include "app_log.h"
#include "app_utils.h"
#include "sampleapp_common.h"

#include "osal.h"
#include "osal_log.h"
#include <bsp-interface/di/console.h>
#include <lwip/netif.h>
#include <pnet_api.h>

#include <string.h>

#define APP_DEFAULT_FILE_DIRECTORY ""
#define APP_LOG_LEVEL APP_LOG_LEVEL_ERROR

#define APP_BG_WORKER_THREAD_PRIORITY 2
#define APP_BG_WORKER_THREAD_STACKSIZE 4096 /* bytes */

/********************************** Globals ***********************************/

static app_data_t *sample_app = nullptr;
static pnet_cfg_t pnet_cfg{};
app_args_t app_args{};

/****************************** Main ******************************************/

int p_net_sample_app_main()
{
	int ret{};
	app_utils_netif_namelist_t netif_name_list{};
	pnet_if_cfg_t netif_cfg{};
	uint16_t number_of_ports{};

	/* Note: station name is defined by app_gsdml.h */
	app_log_set_log_level(APP_DEFAULT_LOG_LEVEL);

	APP_LOG_INFO("\n** Starting P-Net sample application " PNET_VERSION " **\n");
	APP_LOG_INFO("Number of slots:      %u (incl slot for DAP module)\n", PNET_MAX_SLOTS);
	APP_LOG_INFO("P-net log level:      %u (DEBUG=0, FATAL=4)\n", LOG_LEVEL);
	APP_LOG_INFO("App log level:        %u (DEBUG=0, FATAL=4)\n", APP_LOG_LEVEL);
	APP_LOG_INFO("Max number of ports:  %u\n", PNET_MAX_PHYSICAL_PORTS);
	APP_LOG_INFO("Network interfaces:   %s\n", app_args.eth_interfaces);
	APP_LOG_INFO("Default station name: %s\n", app_args.station_name);

	app_pnet_cfg_init_default(&pnet_cfg);

	strcpy(pnet_cfg.file_directory, APP_DEFAULT_FILE_DIRECTORY);

	ret = app_utils_pnet_cfg_init_netifs(app_args.eth_interfaces,
										 &netif_name_list,
										 &number_of_ports,
										 &netif_cfg);

	if (ret != 0)
	{
		return -1;
	}

	pnet_cfg.if_cfg = netif_cfg;
	pnet_cfg.num_physical_ports = number_of_ports;

	app_utils_print_network_config(&netif_cfg, number_of_ports);

	pnet_cfg.pnal_cfg.bg_worker_thread.prio = APP_BG_WORKER_THREAD_PRIORITY;
	pnet_cfg.pnal_cfg.bg_worker_thread.stack_size = APP_BG_WORKER_THREAD_STACKSIZE;

	printf("xPortGetFreeHeapSize = %d\r\n", xPortGetFreeHeapSize());
	printf("xPortGetMinimumEverFreeHeapSize = %d\r\n", xPortGetMinimumEverFreeHeapSize());

	/* Initialize profinet stack */
	sample_app = app_init(&pnet_cfg, NULL);
	if (sample_app == NULL)
	{
		printf("Failed to initialize P-Net.\n");
		printf("Aborting application\n");
		return -1;
	}

	/* Start main loop */
	if (app_start(sample_app, RUN_IN_MAIN_THREAD) != 0)
	{
		printf("Failed to start\n");
		printf("Aborting application\n");
		return -1;
	}

	app_loop_forever(sample_app);
	return 0;
}

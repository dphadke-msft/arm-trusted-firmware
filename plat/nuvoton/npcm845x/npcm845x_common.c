/*
 * Copyright (c) 2015-2020, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>

#include <arch.h>
#include <arch_helpers.h>
#include <common/bl_common.h>
#include <common/debug.h>
#include <drivers/console.h>
#include <lib/debugfs.h>
#include <lib/extensions/ras.h>
#include <lib/mmio.h>
#include <lib/xlat_tables/xlat_tables_compat.h>
#include <plat/arm/common/plat_arm.h>
#include <plat/common/platform.h>
#include <platform_def.h>
#include <drivers/generic_delay_timer.h>

#include <mcr_trustzone.h>
#include <nuvoton_uart_16550.h>
#include <npcm845x_pads.h>
#include <npcm845x_iomux.h>
#include <npcm845x_lpuart.h>
#include <npcm845x_clock.h>
#include <npcm845x_gcr.h>
#include <plat_npcm845x.h>


const mmap_region_t plat_arm_mmap[] = {
//	MAP_DEVICE2,
	MAP_DEVICE0,
	MAP_DEVICE1,
//	MAP_SRAM_NS,
	{0}
};



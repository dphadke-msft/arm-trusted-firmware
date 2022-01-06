/*
 * Copyright (C) 20122 Nuvoton Ltd.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 * https://spdx.org/licenses
 */

#include <common/debug.h>
#include <lib/mmio.h>

#include "mcr_trustzone.h"


#define TRUST_ZONE_BA			0xF0800800
#define TRUST_ZONE_MAX  		188


#define TZ_SET_INDEX	(1<<15)

/* To not duplicate types, the addr_map_win is used, but the "target"
 * filed is referring to attributes instead of "target".
 */
void tz_enable_win(int slave_index, bool set_clear)
{
	
	if ((slave_index < 0) || (slave_index > TRUST_ZONE_MAX))
	{
		ERROR("Enabling wrong MCR TrustZone window %d!\n", slave_index);
		return;
	}
	if (set_clear == true)
		mmio_write_32((4*slave_index) + TRUST_ZONE_BA, mmio_read_32((4*slave_index) + TRUST_ZONE_BA) | TZ_SET_INDEX);
	else
		mmio_write_32((4*slave_index) + TRUST_ZONE_BA, mmio_read_32((4*slave_index) + TRUST_ZONE_BA)  & ~TZ_SET_INDEX);
	
}
	
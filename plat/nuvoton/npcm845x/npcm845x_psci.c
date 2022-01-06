/*
 * Copyright (c) 2015-2018, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdbool.h>

#include <arch.h>
#include <arch_helpers.h>
#include <common/debug.h>
#include <drivers/arm/gicv2.h>
#include <lib/mmio.h>
#include <lib/psci/psci.h>

#include <plat_npcm845x.h>

static const plat_psci_ops_t npcm845x_plat_psci_ops = {
	.cpu_standby = npcm845x_cpu_standby,
	.pwr_domain_on = npcm845x_pwr_domain_on,
	//.pwr_domain_off = npcm845x_pwr_domain_off,
	//.pwr_domain_pwr_down_wfi = npcm845x_pwr_domain_pwr_down_wfi,
	//.pwr_domain_suspend = npcm845x_pwr_domain_suspend,
	.pwr_domain_on_finish = npcm845x_pwr_domain_on_finish,
	//.pwr_domain_suspend_finish = npcm845x_pwr_domain_suspend_finish,
	//.system_off = npcm845x_system_off,
	.system_reset = npcm845x_system_reset,
	.validate_power_state = npcm845x_validate_power_state,
	.validate_ns_entrypoint = npcm845x_validate_ns_entrypoint,
#if !ARM_BL31_IN_DRAM
	.get_sys_suspend_power_state = npcm845x_get_sys_suspend_power_state,
#endif
};
#ifdef TEST
static const plat_psci_ops_t npcm845x_plat_psci_ops = {
	.cpu_standby = npcm845x_cpu_standby,
	.pwr_domain_on = npcm845x_pwr_domain_on,
	.pwr_domain_off = npcm845x_pwr_domain_off,
	.pwr_domain_pwr_down_wfi = npcm845x_pwr_domain_pwr_down_wfi,
	.pwr_domain_suspend = npcm845x_pwr_domain_suspend,
	.pwr_domain_on_finish = npcm845x_pwr_domain_on_finish,
	.pwr_domain_suspend_finish = npcm845x_pwr_domain_suspend_finish,
	.system_off = npcm845x_system_off,
	.system_reset = npcm845x_system_reset,
	.validate_power_state = npcm845x_validate_power_state,
	.validate_ns_entrypoint = npcm845x_validate_ns_entrypoint,
#if !ARM_BL31_IN_DRAM
	.get_sys_suspend_power_state = npcm845x_get_sys_suspend_power_state,
#endif
};

typedef struct plat_psci_ops {
	void (*cpu_standby)(plat_local_state_t cpu_state);
	int (*pwr_domain_on)(u_register_t mpidr);
	void (*pwr_domain_off)(const psci_power_state_t *target_state);
	void (*pwr_domain_suspend_pwrdown_early)(
				const psci_power_state_t *target_state);
	void (*pwr_domain_suspend)(const psci_power_state_t *target_state);
	void (*pwr_domain_on_finish)(const psci_power_state_t *target_state);
	void (*pwr_domain_on_finish_late)(
				const psci_power_state_t *target_state);
	void (*pwr_domain_suspend_finish)(
				const psci_power_state_t *target_state);
	void __dead2 (*pwr_domain_pwr_down_wfi)(
				const psci_power_state_t *target_state);
	void __dead2 (*system_off)(void);
	void __dead2 (*system_reset)(void);
	int (*validate_power_state)(unsigned int power_state,
				    psci_power_state_t *req_state);
	int (*validate_ns_entrypoint)(uintptr_t ns_entrypoint);
	void (*get_sys_suspend_power_state)(
				    psci_power_state_t *req_state);
	int (*get_pwr_lvl_state_idx)(plat_local_state_t pwr_domain_state,
				    int pwrlvl);
	int (*translate_power_state_by_mpidr)(u_register_t mpidr,
				    unsigned int power_state,
				    psci_power_state_t *output_state);
	int (*get_node_hw_state)(u_register_t mpidr, unsigned int power_level);
	int (*mem_protect_chk)(uintptr_t base, u_register_t length);
	int (*read_mem_protect)(int *val);
	int (*write_mem_protect)(int val);
	int (*system_reset2)(int is_vendor,
				int reset_type, u_register_t cookie);
} plat_psci_ops_t;

#endif


int plat_setup_psci_ops(uintptr_t sec_entrypoint,
			const plat_psci_ops_t **psci_ops)
{
	npcm845x_mailbox_init(sec_entrypoint);
	*psci_ops = &npcm845x_plat_psci_ops;

	

	return 0;
}

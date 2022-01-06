/*
 * Copyright (c) 2018, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <assert.h>
#include <stdbool.h>
#include <arch.h>
#include <arch_helpers.h>
#include <common/debug.h>
#include <lib/psci/psci.h>
#include <lib/semihosting.h>
#include <plat/arm/common/plat_arm.h>
#include <plat/common/platform.h>
#include <plat_npcm845x.h>





/*
 * The secure entry point to be used on warm reset.
 */
//static unsigned long secure_entrypoint;
#define ADP_STOPPED_APPLICATION_EXIT 0x20026


/* Make composite power state parameter till power level 0 */
#if PSCI_EXTENDED_STATE_ID

#define npcm845x_make_pwrstate_lvl0(lvl0_state, pwr_lvl, type) \
		(((lvl0_state) << PSTATE_ID_SHIFT) | \
		 ((type) << PSTATE_TYPE_SHIFT))
#else
#define npcm845x_make_pwrstate_lvl0(lvl0_state, pwr_lvl, type) \
		(((lvl0_state) << PSTATE_ID_SHIFT) | \
		 ((pwr_lvl) << PSTATE_PWR_LVL_SHIFT) | \
		 ((type) << PSTATE_TYPE_SHIFT))
#endif /* PSCI_EXTENDED_STATE_ID */


#define npcm845x_make_pwrstate_lvl1(lvl1_state, lvl0_state, pwr_lvl, type) \
		(((lvl1_state) << PLAT_LOCAL_PSTATE_WIDTH) | \
		 npcm845x_make_pwrstate_lvl0(lvl0_state, pwr_lvl, type))



/*
 *  The table storing the valid idle power states. Ensure that the
 *  array entries are populated in ascending order of state-id to
 *  enable us to use binary search during power state validation.
 *  The table must be terminated by a NULL entry.
 */
static const unsigned int npcm845x_pm_idle_states[] = {
	
	//  Cluster = 0 (RUN) CPU=1 (RET, higest in idle) - Retention. The Power state is Stand-by
	npcm845x_make_pwrstate_lvl1(PLAT_LOCAL_STATE_RUN, PLAT_LOCAL_STATE_RET,
				MPIDR_AFFLVL0, PSTATE_TYPE_STANDBY),
	/* State-id - 0x01 */
	
	#ifdef NOT_YET 
	// (RUN) Cluster = 0 (RET higest in idle) CPU=1 (RET) - Retention. The Power state is Stand-by
	npcm845x_make_pwrstate_lvl1(PLAT_LOCAL_STATE_RUN, PLAT_LOCAL_STATE_RET,
				MPIDR_AFFLVL1, PSTATE_TYPE_STANDBY),
	#endif
	0,
};

/*******************************************************************************
 * Platform handler called to check the validity of the non secure
 * entrypoint.
 ******************************************************************************/

 int npcm845x_validate_ns_entrypoint(uintptr_t entrypoint)
{
	/*
	 * Check if the non secure entrypoint lies within the non
	 * secure DRAM.
	 */
	NOTICE("%s() nuvoton_psci\n", __func__);

	if ((entrypoint >= PLAT_ARM_TRUSTED_DRAM_BASE) &&
	    (entrypoint < (PLAT_ARM_TRUSTED_DRAM_BASE + PLAT_ARM_TRUSTED_DRAM_SIZE)))
		return PSCI_E_INVALID_ADDRESS ;
		
			// For TFTS purposes, '0' is also illegal 
	#ifdef SPD_tspd
	
		if (entrypoint == 0)
		{
			return PSCI_E_INVALID_ADDRESS ;
		}
	#endif
	return PSCI_E_SUCCESS;
}

/*******************************************************************************
 * Platform handler called when a CPU is about to enter standby.
 ******************************************************************************/

 void npcm845x_cpu_standby(plat_local_state_t cpu_state)
{

	NOTICE("%s() nuvoton_psci\n", __func__);
	assert(cpu_state == PLAT_LOCAL_STATE_RET);
	
	/*
	 * Enter standby state
	 * dsb is good practice before using wfi to enter low power states
	 */
	dsb();
	wfe();
}

/*******************************************************************************
 * Platform handler called when a power domain is about to be turned on. The
 * mpidr determines the CPU to be turned on.
 ******************************************************************************/
 int npcm845x_pwr_domain_on(u_register_t mpidr)
{
	int rc = PSCI_E_SUCCESS;
	unsigned cpu_id = plat_core_pos_by_mpidr(mpidr);

	if (cpu_id == -1)
	{
		// domain on was not called by a CPU
		ERROR("%s() was not per CPU 0x%X\n", __func__,cpu_id);
		while(1);;
		// Later switch to:
		rc = PSCI_E_INVALID_PARAMS;
	}
	
	if (cpu_id == 0)
	{
		// This cpu is not expected to be turne on!
		// domain on was not called by a CPU
		ERROR("%s() CPU 0 cannot be turned on 0x%X\n", __func__,cpu_id);
		while(1);;
		// Later switch to:
		rc = PSCI_E_INVALID_PARAMS;
	}
 
	if (rc != PSCI_E_SUCCESS)
		return rc;
	
	uint32_t *start_address_pos = (uint32_t *)ARM_CPU_START_ADDRESS(cpu_id);
	NOTICE("%s() nuvoton_psci on CPU 0x%X\n", __func__,cpu_id);
	assert(*start_address_pos == 0);
	assert(*start_address_pos == 0xFFFFFFFF);
	
	
	*start_address_pos = BL31_BASE;
	
	// Make sure that the write has completed
	dsb();
	isb();
	// wake up CPU 
	sev();

	return rc;
}


/*******************************************************************************
 * Platform handler called when a power domain is about to be turned off. The
 * target_state encodes the power state that each level should transition to.
 ******************************************************************************/
 void npcm845x_pwr_domain_off(const psci_power_state_t *target_state)
{
	NOTICE("%s() nuvoton_psci\n", __func__);
	ERROR("NPCM845x Doesnt support Power Down!\n");
	// ON Arbel this is not supported!
	while(1);;

}


 void __dead2
npcm845x_pwr_domain_pwr_down_wfi(const psci_power_state_t *target_state)
{
	NOTICE("%s() nuvoton_psci\n", __func__);
	ERROR("NPCM845x Doesnt support Power Down!\n");
	// ON Arbel this is not supported!
	while(1);;
}

/*******************************************************************************
 * Platform handler called when a power domain is about to be suspended. The
 * target_state encodes the power state that each level should transition to.
 ******************************************************************************/
void npcm845x_pwr_domain_suspend(const psci_power_state_t *target_state)
{
	
	// Arbel downt allow a power domain to be suspended because core 0 
	// will stay active
	NOTICE("%s() nuvoton_psci\n", __func__);
	ERROR("NPCM845x Doesnt support Power Down!\n");
	// ON Arbel this is not supported!
	while(1);;
}


/*******************************************************************************
 * Platform handler called when a power domain has just been powered on after
 * being turned off earlier. The target_state encodes the low power state that
 * each level has woken up from.
 ******************************************************************************/
void npcm845x_pwr_domain_on_finish(const psci_power_state_t *target_state)
{
	NOTICE("%s() nuvoton_psci\n", __func__);
	
	
}


/*******************************************************************************
 * Platform handler called when a power domain has just been powered on after
 * having been suspended earlier. The target_state encodes the low power state
 * that each level has woken up from.
 ******************************************************************************/
void npcm845x_pwr_domain_suspend_finish(const psci_power_state_t *target_state)
{
	NOTICE("%s() nuvoton_psci\n", __func__);
	ERROR("NPCM845x Doesnt support Power Domain!\n");
	// ON Arbel this is not supported!
	while(1);;
}


void __dead2 npcm845x_system_off(void)
{
	NOTICE("%s() nuvoton_psci\n", __func__);
	ERROR("NPCM845x Doesnt support  full system off !\n");
	// ON Arbel this is not supported!
	while(1);;
	
}

void __dead2 npcm845x_system_reset(void)
{
	NOTICE("%s() nuvoton_psci\n", __func__);
	// In the meanwhile  - Discuss wether to reset one core, all cores, which reset etc 
	ERROR("NPCM845 System Reset: operation not handled. yet\n");
	while(1);;
}

int npcm845x_validate_power_state(unsigned int power_state,
			 psci_power_state_t *req_state)
{


	unsigned int state_id;
	int i;
NOTICE("%s() nuvoton_psci\n", __func__);
	assert(req_state);

	/*
	 *  Currently we are using a linear search for finding the matching
	 *  entry in the idle power state array. This can be made a binary
	 *  search if the number of entries justify the additional complexity.
	 */
	for (i = 0; !!npcm845x_pm_idle_states[i]; i++) {
		if (power_state == npcm845x_pm_idle_states[i])
			break;
	}

	/* Return error if entry not found in the idle state array */
	if (!npcm845x_pm_idle_states[i])
		return PSCI_E_INVALID_PARAMS;

	i = 0;
	state_id = psci_get_pstate_id(power_state);

	/* Parse the State ID and populate the state info parameter */
	while (state_id) {
		req_state->pwr_domain_state[i++] = state_id &
						PLAT_LOCAL_PSTATE_MASK;
		state_id >>= PLAT_LOCAL_PSTATE_WIDTH;
	}

	return PSCI_E_SUCCESS;


	
}

/*
 * The NPCM845 doesn't truly support power management at SYSTEM power domain. The
 * SYSTEM_SUSPEND will be down-graded to the cluster level within the platform
 * layer. The `fake` SYSTEM_SUSPEND allows us to validate some of the driver
 * save and restore sequences on FVP.
 */
#if !ARM_BL31_IN_DRAM
void npcm845x_get_sys_suspend_power_state(psci_power_state_t *req_state)
{
	unsigned int i;
NOTICE("%s() nuvoton_psci\n", __func__);
	for (i = ARM_PWR_LVL0; i <= PLAT_MAX_PWR_LVL; i++)
		req_state->pwr_domain_state[i] = PLAT_LOCAL_STATE_OFF;
}
#endif



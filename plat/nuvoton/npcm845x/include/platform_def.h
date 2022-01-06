/*
 * Copyright (c) 2015-2019, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PLATFORM_DEF_H
#define PLATFORM_DEF_H
#include <npcm845x_arm_def.h>



#include <lib/utils_def.h>
#include <plat/arm/common/smccc_def.h>
//#include <plat/arm/css/common/css_def.h>
//#include <plat/arm/soc/common/soc_css_def.h>

#include <arch.h>
#include <common/interrupt_props.h>
#include <common/tbbr/tbbr_img_def.h>
#include <drivers/arm/gic_common.h>
#include <lib/xlat_tables/xlat_tables_defs.h>
#include <plat/common/common_def.h>


#define VALUE_TO_STRING(x) #x
#define VALUE(x) VALUE_TO_STRING(x)
#define VAR_NAME_VALUE(var) #var "="  VALUE(var)




#define DEBUG_MMU_OFF	1
//#undef DEBUG_MMU_OFF
#undef CONFIG_TARGET_ARBEL_PALLADIUM

#define PLATFORM_LINKER_FORMAT		"elf64-littleaarch64"
#define PLATFORM_LINKER_ARCH		aarch64

#define PLATFORM_STACK_SIZE		0x400



/* Local power state for power domains in Run state. */
#define PLAT_LOCAL_STATE_RUN		U(0)
/* Local power state for retention. Valid only for CPU power domains */
#define PLAT_LOCAL_STATE_RET		U(1)
/*
 * Local power state for OFF/power-down. Valid for CPU and cluster power
 * domains.
 */
#define PLAT_LOCAL_STATE_OFF		U(2)


/* Local power state for power domains in Run state. */
//The core is powered up and operational. 
#define ARM_LOCAL_STATE_RUN	 PLAT_LOCAL_STATE_RUN
/* Local power state for retention. Valid only for CPU power domains */
//The core state, including the debug settings, is preserved in low-power structures, allowing 
//the core to be at least partially turned off
#define ARM_LOCAL_STATE_RET	PLAT_LOCAL_STATE_RET
/* Local power state for OFF/power-down. Valid for CPU and cluster power
   domains */
//In this state the core is powered off, In NPCM845x this case is applicable only before 
//cpu was booted 
#define ARM_LOCAL_STATE_OFF	PLAT_LOCAL_STATE_OFF


#define PLATFORM_CORE_COUNT  			NPCM845x_PLATFORM_CORE_COUNT
#define PLATFORM_CLUSTER_COUNT			NPCM845x_CLUSTER_COUNT
#define PLATFORM_MAX_CPU_PER_CLUSTER	NPCM845x_MAX_CPU_PER_CLUSTER
#define PLAT_PRIMARY_CPU				NPCM845x_PLAT_PRIMARY_CPU
#define PLATFORM_SYSTEM_COUNT			NPCM845x_SYSTEM_COUNT





#define PLAT_NUM_PWR_DOMAINS		(PLATFORM_CLUSTER_COUNT + \
					PLATFORM_CORE_COUNT)
#define NPCM845x_MAX_PWR_LVL		ARM_PWR_LVL1


// Hila: Eliminate somehow?
#define PLAT_MAX_RET_STATE		ARM_LOCAL_STATE_RET

/*
 * This macro defines the deepest power down states possible. Any state ID
 * higher than this is invalid.
 */
#define PLAT_MAX_OFF_STATE		ARM_LOCAL_STATE_OFF



/*
 * Macros used to parse state information from State-ID if it is using the
 * recommended encoding for State-ID.
 */
#define PLAT_LOCAL_PSTATE_WIDTH		4
#define PLAT_LOCAL_PSTATE_MASK		((1 << PLAT_LOCAL_PSTATE_WIDTH) - 1)
//#define PLATFORM_CLUSTER1_CORE_COUNT	U(0)


/*
 * Required ARM standard platform porting definitions
 */
#define PLAT_ARM_CLUSTER_COUNT		NPCM845x_PLATFORM_CORE_COUNT


#define PLAT_NUM_PWR_DOMAINS		(PLATFORM_CLUSTER_COUNT + \
					PLATFORM_CORE_COUNT)
#define PLAT_MAX_PWR_LVL		NPCM845x_MAX_PWR_LVL

#define PLAT_LOCAL_PSTATE_WIDTH		4
#define PLAT_LOCAL_PSTATE_MASK		((1 << PLAT_LOCAL_PSTATE_WIDTH) - 1)



//#define PLAT_ARM_TRUSTED_SRAM_SIZE	UL(0x00040000)	/* 256 KB */

//#define PLAT_ARM_TRUSTED_ROM_BASE	UL(0x00000000)
//#define PLAT_ARM_TRUSTED_ROM_SIZE	UL(0x04000000)	/* 64 MB */

#ifndef CONFIG_TARGET_ARBEL_PALLADIUM
	#define PLAT_ARM_TRUSTED_DRAM_BASE	UL(0x3f000000)
#else   
	#define PLAT_ARM_TRUSTED_DRAM_BASE	UL(0xf000000)
#endif
#define PLAT_ARM_TRUSTED_DRAM_SIZE	UL(0x01000000)	/* 16 (was 32) MB */


#define PLAT_NPCM845_HOLD_BASE  UL(0xf0800E00)

#define PWR_DOMAIN_AT_MAX_LVL           U(1)
//#define PLAT_MAX_PWR_LVL                U(1)

#define PLAT_VIRT_ADDR_SPACE_SIZE	(1ull << 32)
#define PLAT_PHY_ADDR_SPACE_SIZE	(1ull << 32)
#define MAX_XLAT_TABLES			16
#define PLAT_ARM_MMAP_ENTRIES	17 
//#define MAX_MMAP_REGIONS		8




#define FIQ_SMP_CALL_SGI  10 		// Hila 

#if RESET_TO_BL31
/* Size of Trusted SRAM - the first 4KB of shared memory */
#define PLAT_ARM_MAX_BL31_SIZE		(PLAT_ARM_TRUSTED_SRAM_SIZE - \
					 ARM_SHARED_RAM_SIZE)
					 
#else
/*
 * Since BL31 NOBITS overlays BL2 and BL1-RW, PLAT_ARM_MAX_BL31_SIZE is
 * calculated using the current BL31 PROGBITS debug size plus the sizes of
 * BL2 and BL1-RW
 */
#define PLAT_ARM_MAX_BL31_SIZE		UL(0x20000)
#endif /* RESET_TO_BL31 */
/*
 * Load address of BL33 for this platform port
 */
#define PLAT_ARM_NS_IMAGE_BASE		(ARM_DRAM1_BASE + UL(0x8000))


//Consider remove 
//#define NPCM845X_TZ1_BASE	0xFFFB0000
#define PLAT_ARM_TRUSTED_SRAM_SIZE    UL(0x00020000)//(0x00040000)	/* 128  KB, the rest 128K if it is non secured */


#define COUNTER_FREQUENCY		0x07735940		//f/4 = 125MHz
#define PLAT_ARM_NSTIMER_FRAME_ID		U(1)

// GIC parameters

/* Base  compatible GIC memory map */
#define NT_GIC_BASE         (0xDFFF8000)
#define BASE_GICD_BASE      (NT_GIC_BASE+0x1000)
#define BASE_GICC_BASE      (NT_GIC_BASE + 0x2000)
#define BASE_GICR_BASE      (NT_GIC_BASE + 0x200000)
#define BASE_GICH_BASE      (NT_GIC_BASE + 0x4000)
#define BASE_GICV_BASE      (NT_GIC_BASE + 0x6000)

#define DEVICE1_BASE			BASE_GICD_BASE
#define DEVICE1_SIZE		0x7000	
//((BASE_GICR_BASE - BASE_GICD_BASE) +	 (PLATFORM_CORE_COUNT * 0x20000))


#define ARM_CPU_START_ADDRESS(m)	UL(0xf0800e00 + 0x10 + m*4)

#define NT_REG_BASE			0xf0000000
#define NT_REG_SIZE			0x0ff16000

// Non-Secured SRAM 
#define NON_SECURE_ON_SRAM

#ifdef NON_SECURE_ON_SRAM
	#define SRAM_NS_BASE			0xFFFD0000
	#define SRAM_NS_SIZE			0x20000
#else
	#define SRAM_NS_BASE			0x3f020000
	#define SRAM_NS_SIZE			0x20000
#endif


// This is for debug purposes!
#define DEVICE2_BASE			0x8000
#define DEVICE2_SIZE			0x10000



// end debug purposes 
/* MMU entry for internal (register) space access */
#define MAP_DEVICE0	MAP_REGION_FLAT(NT_REG_BASE,			\
					NT_REG_SIZE,			\
					MT_DEVICE | MT_RW | MT_SECURE)

#define MAP_DEVICE1	MAP_REGION_FLAT(DEVICE1_BASE,			\
					DEVICE1_SIZE,			\
					MT_DEVICE | MT_RW | MT_SECURE)

#define MAP_DEVICE2	MAP_REGION_FLAT(DEVICE2_BASE,			\
					DEVICE2_SIZE,			\
					MT_MEMORY | MT_RW | MT_NS)
					
					

#define MAP_SRAM_NS	MAP_REGION_FLAT(SRAM_NS_BASE,			\
					SRAM_NS_SIZE,			\
					MT_NON_CACHEABLE | MT_RW | MT_SECURE)


					
// For TSP tests purposes:
#define SRAM2_BASE			0x8000
#define SRAM2_SIZE			0x10000


// Check for redundancy 
//#define PLAT_ARM_NS_IMAGE_BASE	0x0
#define PLAT_ARM_SCP_TZC_DRAM1_SIZE  0x0




/*
 * Define a list of Group 1 Secure and Group 0 interrupt properties as per GICv3
 * terminology. On a GICv2 system or mode, the lists will be merged and treated
 * as Group 0 interrupts.
 */
 
#define PLAT_ARM_GICD_BASE	BASE_GICD_BASE
#define PLAT_ARM_GICC_BASE	BASE_GICC_BASE


#define PLAT_ARM_G1S_IRQ_PROPS(grp) \
	INTR_PROP_DESC(ARM_IRQ_SEC_PHY_TIMER, GIC_HIGHEST_SEC_PRIORITY, grp, \
			GIC_INTR_CFG_LEVEL), \
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_0, GIC_HIGHEST_SEC_PRIORITY, grp, \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_1, GIC_HIGHEST_SEC_PRIORITY, grp, \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_2, GIC_HIGHEST_SEC_PRIORITY, grp, \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_3, GIC_HIGHEST_SEC_PRIORITY, grp, \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_4, GIC_HIGHEST_SEC_PRIORITY, grp, \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_5, GIC_HIGHEST_SEC_PRIORITY, grp, \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_6, GIC_HIGHEST_SEC_PRIORITY, grp, \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_7, GIC_HIGHEST_SEC_PRIORITY, grp, \
			GIC_INTR_CFG_EDGE)

#define PLAT_ARM_G0_IRQ_PROPS(grp)



// Required for compile:

/*
 * PLAT_ARM_MAX_BL1_RW_SIZE is calculated using the current BL1 RW debug size
 * plus a little space for growth.
 */
#define PLAT_ARM_MAX_BL1_RW_SIZE	0 // BOAZ:UL(0xB000)
#if USE_ROMLIB
#define PLAT_ARM_MAX_ROMLIB_RW_SIZE	UL(0x1000)
#define PLAT_ARM_MAX_ROMLIB_RO_SIZE	UL(0xe000)
#define FVP_BL2_ROMLIB_OPTIMIZATION UL(0x6000)
#else
#define PLAT_ARM_MAX_ROMLIB_RW_SIZE	UL(0)
#define PLAT_ARM_MAX_ROMLIB_RO_SIZE	UL(0)
#define FVP_BL2_ROMLIB_OPTIMIZATION UL(0)
#endif

/*
 * PLAT_ARM_MAX_BL2_SIZE is calculated using the current BL2 debug size plus a
 * little space for growth.
 */
#if TRUSTED_BOARD_BOOT
# define PLAT_ARM_MAX_BL2_SIZE	(UL(0x1D000) - FVP_BL2_ROMLIB_OPTIMIZATION)
#else
# define PLAT_ARM_MAX_BL2_SIZE	0// BOAZ: (UL(0x13000) - FVP_BL2_ROMLIB_OPTIMIZATION)
#endif

#undef KOKO
#ifdef KOKO
#define PRINT_ONLY_ONCE
#pragma message(VAR_NAME_VALUE(ARM_AP_TZC_DRAM1_BASE))
#pragma message(VAR_NAME_VALUE(BL31_BASE))
#pragma message(VAR_NAME_VALUE(BL31_LIMIT))
#pragma message(VAR_NAME_VALUE(PLAT_ARM_MAX_BL31_SIZE))
#pragma message(VAR_NAME_VALUE(BL32_BASE))
#pragma message(VAR_NAME_VALUE(BL32_LIMIT))
#pragma message(VAR_NAME_VALUE(PLAT_ARM_MAX_BL32_SIZE)
#pragma message(VAR_NAME_VALUE(SPMD_SPM_AT_SEL2_KKO))

#endif
#define MAX_IO_DEVICES				   4
#define MAX_IO_HANDLES                 4

#define PLAT_ARM_FIP_BASE      	 0x0
#define PLAT_ARM_FIP_MAX_SIZE    PLAT_ARM_MAX_BL31_SIZE    

#define PLAT_ARM_BOOT_UART_BASE 0xF0000000
#define PLAT_ARM_BOOT_UART_CLK_IN_HZ 115200
#define PLAT_ARM_RUN_UART_BASE 0xF0000000
#define PLAT_ARM_RUN_UART_CLK_IN_HZ	115200
#define PLAT_ARM_CRASH_UART_BASE 0xF0000000
#define PLAT_ARM_CRASH_UART_CLK_IN_HZ	115200
#define PLAT_ARM_TRUSTED_MAILBOX_BASE 0xfffB0000
#endif /* PLATFORM_DEF_H */

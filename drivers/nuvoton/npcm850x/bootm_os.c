// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2019-2022 Nuvoton Technology Corp.
 */

#include <common.h>
#include <asm/io.h>
#include <asm/arch/cpu.h>
#include <asm/arch/gcr.h>
#include <asm/arch/info.h>


#define NPCM850_GCR_INTCR2_SELFTEST_PASSED		BIT(11)
#define NPCM850_GCR_INTCR2_WDC				BIT(21)

#define NPCM850_GCR_FLOCKR1_UPDATE_APPROVE		BIT(28)
#define NPCM850_GCR_FLOCKR1_UPDATE_APPROVE_LOCK		BIT(29)

static void npcm850_check_nist_version(void)
{
#if (CONFIG_NIST_VERSION_ADDR != 0)
	volatile u32 uboot_ver = *(u32*)(UBOOT_RAM_IMAGE_ADDR + HEADER_VERSION_OFFSET);
	volatile u32 nist_ver = *(u32*)(CONFIG_NIST_VERSION_ADDR);

	if (uboot_ver != nist_ver)
		reset_cpu(0);
#endif
}

static int npcm850_check_selftest (void)
{
	struct npcm850_gcr *gcr = (struct npcm850_gcr *)npcm850_get_base_gcr();
	int ret = 0;
	int val = 0;

	if (readl(&gcr->intcr2) & NPCM850_GCR_INTCR2_SELFTEST_PASSED)
	{
		val = readl(&gcr->flockr1);
		val |= NPCM850_GCR_FLOCKR1_UPDATE_APPROVE;
		writel(val, &gcr->flockr1);

		/* this will clear INTCR2.WDC */
		reset_misc();

		reset_cpu(0);
	}
	else
	{
		val = readl(&gcr->flockr1);
		val &= ~NPCM850_GCR_FLOCKR1_UPDATE_APPROVE;
		writel(val, &gcr->flockr1);

		val = readl(&gcr->flockr1);
		val |= NPCM850_GCR_FLOCKR1_UPDATE_APPROVE_LOCK;
		writel(val, &gcr->flockr1);
	}
	return ret;
}

/* Allow for arch specific config before we boot */
void arch_preboot_os(void)
{
	/* please define platform specific arch_preboot_os() */
	npcm850_check_nist_version();
	npcm850_check_selftest();
}


/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2006  Ralf Baechle <ralf@linux-mips.org>
 *
 */
#ifndef __ASM_MACH_IP35_DMA_COHERENCE_H
#define __ASM_MACH_IP35_DMA_COHERENCE_H

#include <asm/ip32/crime.h>

struct device;

/*
 * Few notes.
 * 1. CPU sees memory as two chunks: 0-256M@0x0, and the rest @0x40000000+256M
 * 2. PCI sees memory as one big chunk @0x0 (or we could use 0x40000000 for
 *    native-endian)
 * 3. All other devices see memory as one big chunk at 0x40000000
 * 4. Non-PCI devices will pass NULL as struct device*
 *
 * Thus we translate differently, depending on device.
 */

#define RAM_OFFSET_MASK 0x3fffffffUL

static inline dma_addr_t plat_map_dma_mem(struct device *dev, void *addr,
	size_t size)
{
	dma_addr_t pa = virt_to_phys(addr) & RAM_OFFSET_MASK;

	if (dev == NULL)
		pa += CRIME_HI_MEM_BASE;

	return pa;
}

static dma_addr_t plat_map_dma_mem_page(struct device *dev, struct page *page)
{
	dma_addr_t pa;

	pa = page_to_phys(page) & RAM_OFFSET_MASK;

	if (dev == NULL)
		pa += CRIME_HI_MEM_BASE;

	return pa;
}

/* This is almost certainly wrong but it's what dma-ip32.c used to use  */
static unsigned long plat_dma_addr_to_phys(dma_addr_t dma_addr)
{
	unsigned long addr = dma_addr & RAM_OFFSET_MASK;

	if (dma_addr >= 256*1024*1024)
		addr += CRIME_HI_MEM_BASE;

	return addr;
}

static inline void plat_unmap_dma_mem(dma_addr_t dma_addr)
{
}

static inline int plat_device_is_coherent(struct device *dev)
{
	return 0;		/* IP32 is non-cohernet */
}

#endif /* __ASM_MACH_IP35_DMA_COHERENCE_H */

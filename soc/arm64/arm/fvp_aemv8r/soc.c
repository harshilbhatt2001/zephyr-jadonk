/*
 * Copyright (c) 2021 Arm Limited (or its affiliates). All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 */
#include <kernel.h>

#ifdef CONFIG_SOC_FVP_AEMV8R_EL2_INIT

void z_arm64_el2_plat_init(void)
{
	uint64_t reg;

	/*
	 * The purpose of VMPIDR_EL2 is that holds the value of the
	 * Virtualization Multiprocessor ID and this is the value returned
	 * by EL1 reads of MPIDR_EL1.
	 * Normally The VMPIDR_EL2 is configurated by TB-R.
	 */
	reg = read_mpidr_el1();
	write_vmpidr_el2(reg);
}

#endif

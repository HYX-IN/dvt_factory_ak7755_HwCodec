/**
 * bld/memfwprog.lds.cpp
 *
 * Note: This linker script should be preprocessed before used against a
 *	 specific configuration of 'memfwprog' code.
 *
 * History:
 *    2005/02/27 - [Charles Chiou] created file
 *
 * Copyright (c) 2016 Ambarella, Inc.
 *
 * This file and its contents ("Software") are protected by intellectual
 * property rights including, without limitation, U.S. and/or foreign
 * copyrights. This Software is also the confidential and proprietary
 * information of Ambarella, Inc. and its licensors. You may not use, reproduce,
 * disclose, distribute, modify, or otherwise prepare derivative works of this
 * Software or any portion thereof except pursuant to a signed license agreement
 * or nondisclosure agreement with Ambarella, Inc. or its authorized affiliates.
 * In the absence of such an agreement, you agree to promptly notify and return
 * this Software to Ambarella, Inc.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF NON-INFRINGEMENT,
 * MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL AMBARELLA, INC. OR ITS AFFILIATES BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; COMPUTER FAILURE OR MALFUNCTION; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <amboot.h>

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(run_once)
SECTIONS
{
	. = MEMFWPROG_RAM_START;

	.text : {
		__BEGIN_FIRMWARE_IMAGE__ = ABSOLUTE(.);

		run_once*.o (.text)
		*.o (.text*)
		* (.rodata*)
	}

	. = ALIGN(4);
	.data : {
		__nanddb_start = .;
		* (.nanddb)
		__nanddb_end = .;

		. = ALIGN(4);
		* (.data*)
	}

	.rel.plt : {
		. = ALIGN(0x4000);
		*(.rel.plt)
	}

	.bss : {
		. = ALIGN(4);
		__bss_start = .;

		* (.bss)

		__bss_end = .;

		/* Stack for UND mode */
		. = ALIGN(32);
		__und_stack_start = .;
		. = __und_stack_start + 0x100;
		__und_stack_end = .;

		/* Stack for ABT mode */
		. = ALIGN(32);
		__abt_stack_start = .;
		. = __abt_stack_start + 0x100;
		__abt_stack_end = .;

		/* Stack for IRQ mode */
		. = ALIGN(32);
		__irq_stack_start = .;
		. = __irq_stack_start + 0x2000;
		__irq_stack_end = .;

		/* Stack for FIQ mode */
		. = ALIGN(32);
		__fiq_stack_start = .;
		. = __fiq_stack_start + 0x100;
		__fiq_stack_end = .;

		/* Stack for SVC mode */
		. = ALIGN(32);
		__svc_stack_start = .;
		. = __svc_stack_start + 0x100;
		__svc_stack_end = .;

		/* Stack for SYS mode */
		. = ALIGN(32);
		__sys_stack_start = .;
		. = __sys_stack_start + AMBOOT_BLD_STACK_SIZE;
		__sys_stack_end = .;

	}

	.pagetable (NOLOAD) : {
		/* MMU page table */
		. = ALIGN(0x4000);
		__pagetable_main = .;
		. = __pagetable_main + (4096 * 4);	/* 16 KB of L1 */
		__pagetable_hv = .;
		. = __pagetable_hv + (256 * 4);
	}

	. = ALIGN(0x400);						/* must be 1024B align */
	.bldbuf . (NOLOAD) : {
		bld_buf_addr = .;
		. = bld_buf_addr + (384 * SIZE_1KB);
		bld_buf_end = .;
	}

	/DISCARD/ : {
#if !defined(__LINK_BST__)
		.temp/bst*.fw (.text)
#endif
#if !defined(__LINK_BLD__)
		.temp/bld*.fw (.text)
#endif
#if !defined(__LINK_ATF__)
		.temp/atf.fw (.text)
#endif
#if !defined(__LINK_PBA__)
		.temp/pba.fw (.text)
#endif
#if !defined(__LINK_KERNEL__)
		.temp/kernel.fw (.text)
#endif
#if !defined(__LINK_SECONDARY__)
		.temp/secondary.fw (.text)
#endif
#if !defined(__LINK_BACKUP__)
		.temp/backup.fw (.text)
#endif
#if !defined(__LINK_RAMDISK__)
		.temp/ramdisk.fw (.text)
#endif
#if !defined(__LINK_ROMFS__)
		.temp/romfs.fw (.text)
#endif
#if !defined(__LINK_DSP__)
		.temp/dsp.fw (.text)
#endif
#if !defined(__LINK_LNX__)
		.temp/lnx.fw (.text)
#endif
#if !defined(__LINK_SWP__)
		.temp/swp.fw (.text)
#endif
#if !defined(__LINK_ADD__)
		.temp/add.fw (.text)
#endif
#if !defined(__LINK_ADC__)
		.temp/adc.fw (.text)
#endif
	}

	.text : {

#if defined(__LINK_BST__)
		/* Payload of BST */
		. = ALIGN(2048);
		__BEGIN_AMBOOT_BST_IMAGE__ = . - __BEGIN_FIRMWARE_IMAGE__;
		.temp/bst*.fw (.text)
		__END_AMBOOT_BST_IMAGE__ = . - __BEGIN_FIRMWARE_IMAGE__;
#endif

#if defined(__LINK_BLD__)
		/* Payload of BLD */
		. = ALIGN(2048);
		__BEGIN_AMBOOT_BLD_IMAGE__ = . - __BEGIN_FIRMWARE_IMAGE__;
		.temp/bld*.fw (.text)
		__END_AMBOOT_BLD_IMAGE__ = . - __BEGIN_FIRMWARE_IMAGE__;
#endif

#if defined(__LINK_ATF__)
		/* Payload of ATF */
		. = ALIGN(2048);
		__BEGIN_ATF_IMAGE__ = . - __BEGIN_FIRMWARE_IMAGE__;
		.temp/atf.fw (.text)
		__END_ATF_IMAGE__ = . - __BEGIN_FIRMWARE_IMAGE__;
#endif

#if defined(__LINK_PBA__)
		/* Payload of PBA */
		. = ALIGN(2048);
		__BEGIN_PBA_IMAGE__ = . - __BEGIN_FIRMWARE_IMAGE__;
		.temp/pba.fw (.text)
		__END_PBA_IMAGE__ = . - __BEGIN_FIRMWARE_IMAGE__;
#endif

#if defined(__LINK_KERNEL__)
		/* Payload of Kernel */
		. = ALIGN(2048);
		__BEGIN_KERNEL_IMAGE__ = . - __BEGIN_FIRMWARE_IMAGE__;
		.temp/kernel.fw (.text)
		__END_KERNEL_IMAGE__ = . - __BEGIN_FIRMWARE_IMAGE__;
#endif

#if defined(__LINK_SECONDARY__)
		/* Payload of Secondary */
		. = ALIGN(2048);
		__BEGIN_SECONDARY_IMAGE__ = . - __BEGIN_FIRMWARE_IMAGE__;
		.temp/secondary.fw (.text)
		__END_SECONDARY_IMAGE__ = . - __BEGIN_FIRMWARE_IMAGE__;
#endif

#if defined(__LINK_BACKUP__)
		/* Payload of Backup */
		. = ALIGN(2048);
		__BEGIN_BACKUP_IMAGE__ = . - __BEGIN_FIRMWARE_IMAGE__;
		.temp/backup.fw (.text)
		__END_BACKUP_IMAGE__ = . - __BEGIN_FIRMWARE_IMAGE__;
#endif

#if defined(__LINK_RAMDISK__)
		/* Payload of RAMDISK */
		. = ALIGN(2048);
		__BEGIN_RAMDISK_IMAGE__ = . - __BEGIN_FIRMWARE_IMAGE__;
		.temp/ramdisk.fw (.text)
		__END_RAMDISK_IMAGE__ = . - __BEGIN_FIRMWARE_IMAGE__;
#endif

#if defined(__LINK_ROMFS__)
		/* Payload of ROMFS */
		. = ALIGN(2048);
		__BEGIN_ROMFS_IMAGE__ = . - __BEGIN_FIRMWARE_IMAGE__;
		.temp/romfs.fw (.text)
		__END_ROMFS_IMAGE__ = . - __BEGIN_FIRMWARE_IMAGE__;
#endif

#if defined(__LINK_DSP__)
		/* Payload of DSP */
		. = ALIGN(2048);
		__BEGIN_DSP_IMAGE__ = . - __BEGIN_FIRMWARE_IMAGE__;
		.temp/dsp.fw (.text)
		__END_DSP_IMAGE__ = . - __BEGIN_FIRMWARE_IMAGE__;
#endif

#if defined(__LINK_LNX__)
		/* Payload of LNX */
		. = ALIGN(2048);
		__BEGIN_LNX_IMAGE__ = . - __BEGIN_FIRMWARE_IMAGE__;
		.temp/lnx.fw (.text)
		__END_LNX_IMAGE__ = . - __BEGIN_FIRMWARE_IMAGE__;
#endif

#if defined(__LINK_SWP__)
		/* Payload of SWP */
		. = ALIGN(2048);
		__BEGIN_SWP_IMAGE__ = . - __BEGIN_FIRMWARE_IMAGE__;
		.temp/swp.fw (.text)
		__END_SWP_IMAGE__ = . - __BEGIN_FIRMWARE_IMAGE__;
#endif

#if defined(__LINK_ADD__)
		/* Payload of ADD */
		. = ALIGN(2048);
		__BEGIN_ADD_IMAGE__ = . - __BEGIN_FIRMWARE_IMAGE__;
		.temp/add.fw (.text)
		__END_ADD_IMAGE__ = . - __BEGIN_FIRMWARE_IMAGE__;
#endif

#if defined(__LINK_ADC__)
		/* Payload of ADC */
		. = ALIGN(2048);
		__BEGIN_ADC_IMAGE__ = . - __BEGIN_FIRMWARE_IMAGE__;
		.temp/adc.fw (.text)
		__END_ADC_IMAGE__ = . - __BEGIN_FIRMWARE_IMAGE__;
#endif

		. = ALIGN(2048);
		__END_FIRMWARE_IMAGE__ = ABSOLUTE(.);
	}

	.memfwprog . (NOLOAD) : {
		. = ALIGN(0x10000);
		__memfwprog_result = .;
		. = __memfwprog_result  + (0x10000);
		. = ALIGN(0x10000);
		__memfwprog_command = .;
		. = __memfwprog_command + (0x10000);
	}

	.hugebuf . (NOLOAD) : {
		. = ALIGN(0x100000);
		bld_hugebuf_addr = .;
	}

}


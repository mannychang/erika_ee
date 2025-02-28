/*
 * Name: ee_tc29x_gnu_memory_cpu0.x
 *
 * Description: TC29x memory description for cpu0 in multicore
 * environment
 */

/*
 * Global
 */

OUTPUT_FORMAT("elf32-tricore")
OUTPUT_ARCH(tricore)
ENTRY(EE_tc2Yx_cpu1_start)

 __TRICORE_DERIVATE_MEMORY_MAP__ = 0x290;

/* Start-up Program Flash Memory */
__PMU_PFLASH_STARTUP_BEGIN = 0x80000000;
__PMU_PFLASH_STARTUP_SIZE  = 1k;

/* Absolute 18bit addressing data */
__PMU_PFLASH_ABS18_BEGIN  = 0x80002000;
__PMU_PFLASH_ABS18_SIZE   = 4k;

/*Program Flash Memory (PFLASH1)*/
__PMU_PFLASH_BEGIN = 0x80400000;
__PMU_PFLASH_SIZE  = 2M;

/*Data Flash Memory (DFLASH0)*/
__PMU_DFLASH0_BEGIN = 0xAF080000;
__PMU_DFLASH0_SIZE = 256k;

/*Data Flash Memory (DFLASH1)*/
__PMU_DFLASH1_BEGIN = 0xAF118000;
__PMU_DFLASH1_SIZE = 16K;

/*Boot ROM (BROM)*/
__BROM_BEGIN = 0x8FFF8000;
__BROM_SIZE = 32K;

/*Scratch-Pad RAM (PSPR)*/
__PMI_PSPR_BEGIN = 0x60100000;
__PMI_PSPR_SIZE = 32K;
/*Local Data RAM (DSPR)*/
__DMI_DSPR_BEGIN = 0x60000000;
__DMI_DSPR_SIZE = 240K;

/*Data RAM (LMU)*/
__LMU_SRAM_BEGIN = 0x90004000;
__LMU_SRAM_SIZE = 8K;

  /* CSA list size */
__CSA_SIZE = DEFINED (__CSA_SIZE) ? __CSA_SIZE : 8k ;
  /* User stack Size */
__USTACK_SIZE = DEFINED (__USTACK_SIZE) ? __USTACK_SIZE : 2k;
  /* Interrupt Stack Size */
__ISTACK_SIZE = DEFINED (__ISTACK_SIZE) ? __ISTACK_SIZE : 0k;
  /* Heap size */
__HEAP_SIZE = DEFINED (__HEAP_SIZE) ? __HEAP_SIZE : 0;

MEMORY
{
  /* - 0x(8)A0000000--0x(8)A000001F ABM Header Start */
  /* - 0x(8)A0000020 Boot Address */
  /* - 0x(A)80000020--0x(A)80017FFF Code Application Pattern */

  /* - Program Flash Memory for startup (PFLASH0) */
  PMU_PFLASH_STARTUP (rx!p):	org = 0x80000000, len = 1k

  /* - Program Flash Memory for 18 bit absolute addressing (PFLASH0) */
  PMU_PFLASH_ABS18 (rx!p):	org = 0x80002000, len = 4k
  /* - Program Flash Memory (PFLASH1) */
  PMU_PFLASH (rx!p):		org = 0x80400000, len = 2M
  /* - Data Flash Memory (DFLASH0) */
  PMU_DFLASH0 (r!xp):		org = 0xAF080000, len = 256k
  /* - Data Flash Memory (DFLASH1) */
  PMU_DFLASH1 (r!xp):		org = 0xAF118000, len = 16k

  /* - Boot ROM (BROM) */
  BROM (rx!p):			org = 0x8FFF8000, len = 32k

  /* - Scratch-Pad RAM (PSPR) */
  PMI_PSPR (wx!p):		org = 0x60100000, len = 32k
  /* - Local Data Scratch-Pad RAM (DSPR) */
  DMI_DSPR (w!xp):		org = 0x60000000, len = 240k
  /* - Data RAM */
  LMU_SRAM (w!xp):		org = 0x90004000, len = 8K
}

/*
REGION_ALIAS("DATA_MEM", int_dram)
REGION_ALIAS("CODE_MEM", int_cflash)
REGION_ALIAS("SDATA_MEM", int_dram)
REGION_ALIAS("BSS_MEM", int_dram)
REGION_ALIAS("ZDATA_MEM", int_dram)
REGION_ALIAS("CSA_MEM", int_dram)
 dummy mapping (needed by default linker script)
REGION_ALIAS("PCP_CODE", int_cflash)
REGION_ALIAS("PCP_DATA", int_dflash)
*/

/* the symbol __TRICORE_DERIVATE_NAME__ will be defined in the crt0.S and is
 * tested here to confirm that this memory map and the startup file will
 * fit together
*/
/*
_. = ASSERT ((__TRICORE_DERIVATE_MEMORY_MAP__ == __TRICORE_DERIVATE_NAME__), "Using wrong Memory Map. This Map is for TC27x");
*/
/* INSERT BEFORE .startup */

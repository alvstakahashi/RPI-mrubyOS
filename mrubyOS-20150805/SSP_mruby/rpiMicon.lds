heap_size = 128M;
/*heap_size = 1M;*/
/* heap_size = 1048576; */


OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
	. = 0x8000;

	.text : { *(.text*) }
	. = ALIGN(4);

	__rodata_start = .;
	.rodata : { *(.data*) }
	. = ALIGN(4);
	__rodata_end = .;

	__data_start = . ;
	.data : { *(.data*) }
	. = ALIGN(4);
	__data_end = . ;

	__bss_start = . ;
	.bss : { *(.bss*) }
	. = ALIGN(4);
	__bss_end = . ;
	. = ALIGN(4096);
	__free_start = .;

	. = 0x06400000;
    
    .heap :
    {
   . = ALIGN(4);
   __heap_start__ = .;
   *(.heap*)
 . += heap_size;
   . = ALIGN(4);
__heap_limit__ = .;
    } 
}

__mem_end__ = .;

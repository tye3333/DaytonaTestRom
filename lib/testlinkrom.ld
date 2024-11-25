OUTPUT_ARCH( "KB" )

MEMORY
{
        rom: o=0x00000000,l=0x100000
        ram: o=0x00500000,l=0x20000
}

SECTIONS
{
	.text :
        {
        } >rom

	.data : 
	{
	_ram = .;
	} >ram

	.bss  : 
	{
	} >ram
}

cs1 = -(system_address_table + prcb_ptr + start_ip);

_heap_size = 0x20000;
_heap_base = (_end + 0xf) & ~0xf;
_heap_end = _heap_base + _heap_size - 1;

STARTUP (kx_init.o)


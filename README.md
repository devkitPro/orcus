# orcus
Low level library for GP2X

<pre><code>
To launch an elf executable via JTAG...

sudo openocd -f gp2x-openocd.cfg

$DEVKITARM/bin/arm-none-eabi-gdb
exec-file YOUR.elf
target remote localhost:3333
load
cont
</code></pre>

After recompiling, just run ctrl+c to kill the process within gdb then 'load' and 'cont' again.

# How to define memory map

If the following symbols are declared (e.g. in ldflags -Xlinker --defsym=__testSymbol=64), they will be used.

| Symbol | Purpose |
| --- | --- |
| __int_stack_size | number of bytes allocated at top of RAM for interrupt stack (x2, one for FIQ, one for IRQ) |
| __usr_stack_size | number of bytes allocated at top of RAM for user stack |
| __non_heap_allocation | additional bytes reserved underneath the stack for things like TLBs |
| __io_base | base address of IO registers |
| __nand_io_base | base address of NAND IO registers |
| __end_of_heap | pointer to end of available heap space |
| __io_base | base address of IO registers |
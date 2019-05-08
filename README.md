# orcus
Low level library for GP2X

To launch an elf executable via JTAG...

sudo openocd -f gp2x-openocd.cfg

$DEVKITARM/bin/arm-none-eabi-gdb
exec-file YOUR.elf
target remote localhost:3333
load
cont

After recompiling, just run ctrl+c to kill the process within gdb then 'load' and 'cont' again.
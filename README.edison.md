# uCore-plus在Edison开发板的运行

## uboot的定制

uboot中需要加入载入E820表和载入ELF到内存并执行的功能。

在出现`GADGET DRIVER: usb_dnl_dfu`时，通过

	sudo dfu-util -v -d 8087:0a99 --alt u-boot0 -D u-boot-edison-2014.04-1-r0.bin
	
可以刷入新的uboot，加载运行后可以看到`U-Boot 2014.04 (Apr 15 2015 - 01:19:51)`的字样，表明是新的uboot。

刷入新的uboot以后，先引导到Linux，拷贝要加载的kernel到分区，假设kernel名为`kernel-i386-new.elf`，则在重启后在uboot下运行：

	loade820
	bootu mmc 0:9 kernel-i386-new.elf
	
即可加载uCore。

## uCore的修改

### ELF Program Headers
首先编译以后的ELF需要进行修改。具体而言，ELF各个Program Header的p_pa（physical addr）实际上是错误的，是虚拟地址。而在ucore自己的bootloader中屏蔽了物理地址的高位得到了正确的值，但uboot中并没有也不应该做这样的事情。

所以需要写一个程序修改kernel的ELF的header使其值正确，具体可以参考uCore的bootloader和uboot的bootelf指令的实现。

### 串口输出方式
Edison的串口输出是UART的MMIO，通过对地址0xff010180进行读写。

另外需要注释掉原先串口初始化和输出的部分，否则似乎会影响UART。

### 关闭Watchdog
Edison的watchdog需要关闭，否则会定时重置Edison。

	movl 0x10f8 to 0xff009000

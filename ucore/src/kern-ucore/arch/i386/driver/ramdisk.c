#include <string.h>
#include <memlayout.h>
#include <assert.h>
#include <stdio.h>
#include <kio.h>
#include <fs.h>
#include <ramdisk.h>

bool check_initrd()
{
	if (initrd_begin == initrd_end) {
		kprintf("Warning: No Initrd!\n");
		return 0;
	}
	kprintf("Initrd: 0x%08x - 0x%08x, size: 0x%08x, magic: 0x%08x\n",
		initrd_begin, initrd_end - 1, initrd_end - initrd_begin,
		*(uint32_t *) initrd_begin);
	return 1;
}

int ramdisk_read(unsigned long secno, void *dst,
			unsigned long nsecs)
{
    assert(nsecs <= (INITRD_SIZE / SECTSIZE));
    assert((secno + nsecs) * SECTSIZE < INITRD_SIZE);
	assert(nsecs >= 0);
	memcpy(dst, (void *)(DISK_FS_VBASE + secno * SECTSIZE), nsecs * SECTSIZE);
	return 0;
}

int ramdisk_write(unsigned long secno,
			 const void *src, unsigned long nsecs)
{
	//kprintf("%08x(%d) %08x(%d)\n", dev->lba, dev->lba, secno, secno);
    assert(nsecs <= (INITRD_SIZE / SECTSIZE));
    assert((secno + nsecs) * SECTSIZE < INITRD_SIZE);
	assert(nsecs >= 0);
	memcpy((void *)(DISK_FS_VBASE + secno * SECTSIZE), src, nsecs * SECTSIZE);
	return 0;
}

unsigned long ramdisk_size()
{
    return INITRD_SIZE;
}

void ramdisk_init()
{
	kprintf("ramdisk_init(): initrd found, magic: 0x%08x, 0x%08x secs\n",
		*(uint32_t *)DISK_FS_VBASE , INITRD_SIZE / SECTSIZE);

}

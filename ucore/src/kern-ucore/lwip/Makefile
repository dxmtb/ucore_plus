TARGET_CFLAGS = -fno-builtin -nostdlib  -nostdinc -m32 -g -ggdb
KERN_INCLUDES = lwip/include lwip/include/ipv4 lwip/include/ lwip/sys libs sync arch/$(ARCH)/sync arch/$(ARCH)/libs arch/$(ARCH)/driver arch/$(ARCH)/mm schedule
TARGET_CFLAGS += $(addprefix -I$(KTREE)/, $(KERN_INCLUDES))
dirs-y := api core netif sys

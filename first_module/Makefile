include ../vars.mk

KBUILD = $(MAKE) -C "$(KERNEL_DIR)" M=$(CURDIR)
MODULE_NAME = first_module
# used by Kbuild
obj-m += $(MODULE_NAME).o


.PHONY: all build load unload clean $(MODULE_NAME).ko

all: build

## build the module using KBuild
build:
	$(KBUILD) modules

## load the module
load: build
	@# unload previous version, if any
	@if lsmod | grep "^$(MODULE_NAME)\s" --quiet; then $(MAKE) unload; fi
	insmod "$(MODULE_NAME).ko"

## unload the module
unload:
	rmmod "$(MODULE_NAME)"

clean:
	$(KBUILD) clean

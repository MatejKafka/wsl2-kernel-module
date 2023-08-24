include ../vars.mk

MODULE_NAME = first_module

# shorthand to invoke the Kbuild Makefile, which will do the actual operation for us
KBUILD = $(MAKE) -C "$(KERNEL_DIR)" M=$(CURDIR)
# used by Kbuild, list of module object files
obj-m += $(MODULE_NAME).o


.PHONY: all build load unload clean

all: build

# build the module using Kbuild
build:
	$(KBUILD) modules

# load the module
load: build
	@# unload previous version, if any
	@if lsmod | grep "^$(MODULE_NAME)\s" --quiet; then $(MAKE) unload; fi
	insmod "$(MODULE_NAME).ko"

# unload the module
unload:
	rmmod "$(MODULE_NAME)"

clean:
	$(KBUILD) clean

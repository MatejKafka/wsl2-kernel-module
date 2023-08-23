include vars.mk
.PHONY: all install-dependencies build-kernel

all: build-kernel

## install required dependencies for building the kernel
install-dependencies:
	@# Ubuntu/Debian is expected
	apt install bison build-essential flex libssl-dev libelf-dev bc pahole

## clone the WSL linux kernel repo matching the installed kernel version
$(KERNEL_DIR):
	@# if downloading the repo, also check that we have all required dependencies
	$(MAKE) install-dependencies
	@# WSL kernel has tags for each version
	git clone --branch "linux-msft-wsl-$(KERNEL_VERSION)" --depth 1 https://github.com/microsoft/WSL2-Linux-Kernel.git "$(KERNEL_DIR)"	

## setup the kernel build config file, using the provided defaults for WSL
## (it is important that we build the kernel with the same config as the running kernel, otherwise our modules would be refused)
$(KERNEL_DIR)/.config: | $(KERNEL_DIR)
	cp "$(KERNEL_DIR)/Microsoft/config-wsl" $@
	@# yes '' to accept defaults for any new options
	yes '' | $(MAKE) -C "$(KERNEL_DIR)" prepare

## build the kernel
build-kernel: $(KERNEL_DIR)/.config
	$(MAKE) -C "$(KERNEL_DIR)" -j $(shell nproc)
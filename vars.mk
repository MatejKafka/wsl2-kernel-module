# absolute path to the project dir
# https://stackoverflow.com/a/8080530
PROJECT_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

# version number for the kernel
KERNEL_VERSION = $(shell uname -r | cut -d- -f1)
# directory where the Linux kernel is cloned and built
KERNEL_DIR = $(PROJECT_DIR)/WSL2-Linux-Kernel
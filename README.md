# WSL2 Linux kernel module development sample

A small project to setup a Debian/Ubuntu-based WSL2 instance for Linux kernel module development.



## Why is this necessary?

While starting with kernel module development on most Linux distributions, you just need to obtain a `linux-headers-*` package and you're golden.

With WSL2, the issue is that the WSL kernel is shared between all WSL2 instances and managed separately from the userspace. Thus, the version of the kernel (`uname -r`) will typically be different from what the distribution userspace was originally built for, and installing the `linux-headers-*` package will install headers for the wrong kernel version.

To compile kernel modules for the WSL2 kernel, we will need to download and build the correct kernel version ourselves.



## Usage

1. Clone this repository:
  ```sh
  git clone github.com/MatejKafka/wsl2-kernel-module
  ```

2. Install dependencies, clone and build the WSL2 kernel for your current kernel version. This step will take some time (~15 minutes on my laptop):
  ```sh
  cd wsl2-kernel-module
  make
  ```

3. Compile the "hello world" kernel module:
  ```sh
  # compile and load the module
  make -C first_module load
  # unload it
  make -C first_module unload
  ```

4. Check that the module was correctly loaded and unloaded:

  ```sh
  # view the last 2 messages in the kernel log
  dmesg | tail -n 5
  # ...
  # [ 1290.261435] first_module: module loaded
  # [ 1290.267202] first_module: unloading module
  ```
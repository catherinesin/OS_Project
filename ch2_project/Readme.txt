Environment：Linux

Instructions:
1. make
   to compile the module, and the file simple.ko represents the compiled kernel module
2. sudo insmod simple.ko
   to load the kernel module
3. lsmod 
   list the current linux kernel modules in three columns: name, size, where the module is being used
   enter to check whether the module has loaded
4. dmesg 
   to print content of the message in the kernel log buffer
5. sudo rmmod simple 
   to remove kernel module
6. sudo dmesg -c
   clear the kernel log buffer periodically
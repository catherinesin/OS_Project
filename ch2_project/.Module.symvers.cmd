cmd_/home/cskt/Desktop/os_linux/ch2_project/Module.symvers := sed 's/\.ko$$/\.o/' /home/cskt/Desktop/os_linux/ch2_project/modules.order | scripts/mod/modpost -m -a  -o /home/cskt/Desktop/os_linux/ch2_project/Module.symvers -e -i Module.symvers   -T -

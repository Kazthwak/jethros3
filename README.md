# JETHROS3
After version 2 reached an unsustainable quantity of technical debt, I have once again restarted completely (and reached a further
point in far less time) <br/>
This OS is a higher half kernel, with basic memory management <br/>
My hope is that in the not-too-distant future, I will add usermode and proper program loading. Then will come multithreading, which will be an interesting challenge,
that I am both dreading and looking forward to. (Although the chances are that will be in version 4).

Current features: <br/>
Can load programs, and includes rudimentary syscall code.
And the programs are loaded in user mode!!!
<br/>
FAT library: (That I somehow made work): https://github.com/ultraembedded/fat_io_lib/tree/master. <br/>
This is currently read-only, and updating it is low on my priority list, as the disc is overwritten every compile, so writes wouldn't be very useful. <br/>
This must be compiled so that it can be used (and put in a folder named FAT so that the OS can access the headers). A command to do that is in the makefile
<br/><br/>
I have had some (1 or 2) questions about how to load it. I may be misinterpreting it, but I believe the license on GRUB means I cannot simply distribute the disc file (which includes GRUB), so no disc image is provided. In order to run JETHROS, one must either contact me for a copy of the disc image, or compile it onself (probably quite difficult as it requires a linux machine with the same cross compiler location and to build the FAT lib)

# JETHROS3
After version 2 reached an unsustainable quantity of technical debt, I have once again restarted completely (and reached a further
point in far less time) <br/>
This OS is a higher half kernel, with basic memory management <br/>
My hope is that in the not-too-distant future, I will add proper FAT support (as opposed to the basic driver that can almost find the
location of files), and then usermode and proper program loading. Then will come multithreading, which will be an interesting challenge,
that I am both dreading and looking forward to. (Although the chances are that will be in version 4).

Current features: <br/>
Nothig of note yet, this is just a project. Once it can load programs, this section will be populated. This will be V0.3
<br/>
FAT library: (That I somehow made work): https://github.com/ultraembedded/fat_io_lib/tree/master. <br/>
This is currently read-only, and updating it is low on my priority list, as the disc is overwritten every compile, so writes wouldn't be very useful. <br/>
This must be compiled so that it can be used (and put in a folder named FAT so that the OS can access the headers). A command to do that is in the makefile

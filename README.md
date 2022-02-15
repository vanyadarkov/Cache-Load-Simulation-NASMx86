# Cache load simulation nasm x86
## Introduction 
When it has to work with some data, the processor must first bring it from **RAM** to registers (its internal memory). **RAM** has the advantage of being able to store large amounts of data, but it takes relatively long for the processor to access data from RAM. Instead, the registers can be accessed very quickly (they are part of the processor), but they are small in size and we cannot store a lot of data in them simultaneously.
**Cache** memory is a compromise solution used in almost all modern computing systems. This is an additional memory unit, placed between the **RAM** and the processor. Its size is larger than that of the registers, but smaller than that of the RAM. Also, the access time is better than the data access time in RAM, and this helps us to shorten the time to bring data from RAM to the processor.
When the processor needs data from a specific address in **RAM**, it first caches it to see if that data exists there. If so, it means that we have a **CACHE HIT** and we can retrieve the data directly from the cache, without interacting with the main memory, thus saving time. If the data does not yet exist in the cache, it means that we have a **CACHE MISS**, in which case the data must first be brought from the cached **RAM**, after which it must be transferred to the processor registers. Although we have an additional delay in a **MISS CACHE**, the second time we try to access data from the same address we will be able to access it faster, because it is now in the cache (provided it has not been overwritten in the meantime!).
## Project structure

 - Input files - represent an area of RAM with data
 - Output folder - where the outputs will be after running
 - Ref folder - what the output should look like
 - cache.asm - the asm file in which the load in cache function is
   implemented
 - Makefile
 - checker.c, checker.h - for reading input, writing, checking with ref
   results
## Cache structure (for this project)
 - The cache for this task is **simplified** and has the array with **100** lines, each line having **8** bytes.
 - Each line in the **cache** has a tag associated with it. If that line does not contain data, the tag is 0. If the line in the cache contains data, the tag is equal to the value obtained from the first 32 - 3 = 29 bits of the address of the first byte on that line.
 - The tag is used to search for data from a specific cached address. In implementation, tags are represented as a vector with CACHE_LINES elements
## Load algorithm

    void load(char *reg, char** tags, char **cache, char *address, int to_replace)

- **reg** - The "register" in which we want to load data will actually be a 1 byte variable, char reg. Basically, we'll be interested in receiving a memory address and finally loading a byte from that address in reg.
- **address** is the address of the byte that must be finally brought to **reg**
- **to_replace** is the cache line index where we need to bring data in case of **CACHE_MISS**

The load algorithm to be implemented is as follows:

1. We calculate the tag for the address from which we want to obtain data
2. We iterate through the tags of each line. If we find a line that has the associated tag calculated in step 1, we calculate the offset within the line for the byte we want to copy to the register and bring this byte from the cache to the register. (The offset is represented by the least significant 3 bits in the byte address, the ones we cut when calculating the tag.)
3. If we have not found a line that has the associated tag calculated in step 1, we need to bring 8 consecutive bytes of memory into a cache line, one of these bytes being the desired one (**CACHE MISS**). The line we need to write in this step is the **to_replace** index. Once the line has been cached, the register must be written as in the previous step.

#### Copyright
The copyright for the checker.c, checker.h and, input, ref files belongs to the IOCLA team, Universitatea Politehnica din Bucuresti, 2021. Load implementation from cache.asm belongs to [vanyadarkov](https://github.com/vanyadarkov).
# StackAllocator
Dynamic memory allocation is a staple of software engineering.
It is best to avoid dynamic memory whenever possible, however
it is often not possible to avoid. For most applications
the default methods of allocating memory are more than sufficient,
however high performance applications can suffer performance
issues due to the fact that new and alloc/malloc require
kernel calls to complete.

To avoid performance issues custom memory allocators can
avoid constant kernel calls by pre-allocating a large chunk
of memory and allocating user memory from there. This has
the advantage of only calling kernel code once, and also
allows the memory allocator to be fine tuned to specific
data structures.

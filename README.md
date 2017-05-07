--David Olson

Virtual Memory Simulator

This project is a naive implementation of virtual memory. We are not considering protection bits, and not actually reading/writing/accessing disk.

In order to run call make and provide the following command line input:

  ./vmm [-p num_pages] [-f num_frames] [-b block_size] [-r rep_policy] <inst_file>


Where where the flags and information are optional and default to:

num_pages - (optional)(defaults to 8, if not given).
num_frames - (optional) usually a multiple of 2 (defaults to 4, if not given).
block_size - (optional) must be a multiple of 2 and is the number of bytes per
rep_policy (optional) is the page replacement policy. You should allow the following random  fifo  lru  clock (Defaults random if not given).
inst_file (required) is a path to a file containing instructions.



instructions are assumed to be inputted correctly:

W 0x00000000 //write
R 0x00000400 //read
PF //print frame
PP //print page

ex)
W 0x00000000
R 0x00000400
W 0x00000800
W 0x00000000
R 0x00000400
R 0x00000C00
W 0x00000000
R 0x00000C00
R 0x00000400
W 0x00000800
R 0x00000400
PF
PP

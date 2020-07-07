# 5-1_Computer_Achitecture

### PA1, PA2, PA3 is making MIPS-simulator.

##### read <filename>
  It reads binary file and shw the disassembled instruction.
  
##### exit
  finish the shell loop.
  
##### loadinst <filename>
  It reads binary instructions file and store the binary instructions to the simulated instruction memory.
 
##### loaddata <filename>
  It reads binary file and store the binary data to the data memory. (Start from 0x10000000)
  
##### run <N>
  It simulate up to number of N instructions. (Start from PC address of 0x00000000)
  
##### registers
  It show current value of registers. ($0 - $31, HI, LO, and PC)
  
##### *Error Handling

1. Memory address out of range
2. Memory address alignment error

### PA4 is making LRU cache simulator.


##### 1. Cache Type 0 is 16KB of total data capacity, 16 bytes per block, Direct-mapped, Write-through, no write allocate policy, 32-bit memory address.

##### 2. Cache Type 1 is 64KB of total data capacity, 64 bytes per block, 2-way set associative, LRU replacement policy, Write-back, write allocate policy, 32-bit memory address.

# GraphingCalculator452

### How to set it up:
1. use vitis 2020.2, create a new platform
2. use the included bitstream (.xsa) file to set up the hardware
3. in the BSP create another domain to access the second processor(note for CPU1: add this flag: -DUSE_AMP=1 and enable FAT file system access for the SDCARD reader
4. build the HW platform
5. create a software application and make sure its in a C environment
6. add an additional application and make sure its in a C++ environment (Audio CPU0, Video CPU1)
7. copy and paste all the code include the scipt.ld files(they include all the memory settings)
8. build the application project and program it onto the board.

On-screen Keyboard:<ul>
![image](https://github.com/user-attachments/assets/c0bd21d9-2ec3-4d32-afba-bb7bfc2e6f10)<ul>
Log_2 Graph Result:<ul>
![image](https://github.com/user-attachments/assets/d615533c-f5ba-4d12-ac77-151b1dd732a8)<ul>
Parabola?!:<ul>
![image](https://github.com/user-attachments/assets/f8bb7c9e-b6c5-4aa4-a06e-3b85bcc21e75)<ul>

I dont currently have my own zedboard anymore as it was rented but im hoping to expand the project in the future :P

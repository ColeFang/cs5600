1) What fields did you have to add 
For the inode structure, I add a pointer argument which points to the corresponding data block, and bytes argument denoting the size of each file.

2) What is the maximum size of a file supported by your inode structure?  Show your work.

the maximum size for a file is (126+128+128\*128)\*SECTOR_SIZE = 8319KB,  

3) Is your inode structure a multilevel index?  If so, why did youchoose this particular combination of direct, indirect, and doubly
indirect blocks?  If not, why did you choose an alternative inode structure, and what advantages and disadvantages does your structure have, compared to a multilevel index?

my inode structure is a multilevel index, I choose this because this structure is convenient to write and read large files.
when a file is less than 126\*SECTOR_SIZE, the inode points to the data block directly,
when a file is larger than 126\*SECTOR_SIZE but smaller than (126+128)\*SECTOR_SIZE, the file will be stored using single indirect block,
when a file is larger (126+128)\*SECTOR_SIZE, the file will be stored using double indirect block,

4) Describe your code for traversing a user-specified path.  How do traversals of absolute and relative paths differ?
The user path will be split by "/", for each level of dirs, the program will look up this dir level in the corresponding block(start from block 0) and find the inode number for it. Then the program will find this inode in disk.inodeTable and go into the new block using the inode.points. The program will look up the next level of dirs in this new block until the last level. 

Absolute path can be directly used by the program. 
Relative path needs to be attached to the current path.

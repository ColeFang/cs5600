1.Describe your design of the page table and frame tables. How and why did you store your data the way you did?

I build a dynamic array as the frame table. I also defined a frame structure as the element of this array.

I build a 2D dynamic array as the page table, the first dimension is the id of threads and the second dimension is the id of the page. I also defined a page structure as the element of this array.

2.Describe your algorithm for finding the frame associated with a page.

each page object contains a argument called frameId, which denote the frame associated to this page. When the allocate memory function is called, the function will allocate frames to every page by changing frameId. When the read function is called, it will find a corresponding frame according to the frameId. 

3.When two user processes both need a new frame at the same time, how are races avoided?

I add a mutex lock to ensure that a frame can only be load by one process at the same time. If two threads load the frame at the same time, the thread with lower thread id will get it first. 

4.Why did you choose the data structure(s) that you did for representing virtual-to-physical mappings?

I put an arguments to the page structure and frame structure to represent their relationships rather than using maps. It is more flexible to use.

5.When a frame is required but none is free, some frame must be evicted.  Describe your code for choosing a frame to evict.

I defined a index=0. When all the frame is used, the program will choose the frame at this index: frame_table[index], and index+=1 after a frame is evicted. 

6.When a process P obtains a frame that was previously used by a process Q, how do you adjust the page table (and any other data structures) to reflect the frame Q no longer has?

If the frame was previously used by process Q, process P will evict this frame, swap its data to disk and then write to this frame. During this progress, the corresponding page in process Q will be modified: the argument filepath will be set from NULL to the file address in the disk.  In the next time when the process Q wants to obtain this page, it will check the filepath argument, if it is not NULL, the process Q will know it no longer has this frame and trying to read from disk or swap back to the memory. 

7.A page fault in process P can cause another process Q's frame to be evicted.  How do you ensure that Q cannot access or modify the page during the eviction process?  How do you avoid a race between P evicting Q's frame and Q faulting the page back in?

The eviction process is in the write and read function, and all the write and read functions are protected by the mutex lock. So other process cannot access or modify the page during the eviction process.

In the next time when process Q trying to read this page, it will read directly from disk according to page.filepath rather than trying to fault the page back in. 

8.Suppose a page fault in process P causes a page to be read from the file system or swap.  How do you ensure that a second process Q cannot interfere by e.g. attempting to evict the frame while it is still being read in?

The read process is protected by a mutex lock, other process cannot interfere it.

9.A single lock for the whole VM system would make synchronization easy, but limit parallelism.  On the other hand, using many locks complicates synchronization and raises the possibility for deadlock but allows for high parallelism.  Explain where your design falls along this continuum and why you chose to design it this way.

I set a mutex lock on each frame, therefore different frame can be write or read at the same time. 

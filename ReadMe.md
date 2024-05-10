# Project YALL

## problem statement
```
Create a C++ program that implements a linked list. The linked list should be 
a doubly linked list, allowing movement forward and backward. 
- This program should allow you to add and remove nodes from the list. 
- Each node should contain a reference to application data. 
- The program does not have to provide user interaction. 
- Please include unit tests for the program. 

When completed, the program should be sent to your designated recruiter by 
including a link to your solution in GitHub in an email.
```

## building
The project uses cmake to build and will fetch [Googletest](https://github.com/google/googletest) from GitHub (internet connection required for the build phase).
Testing is enabled by default, do disable:
```
> mkdir .build && cd .build
> cmake .. -DBUILD_YALL_TESTS=0
```
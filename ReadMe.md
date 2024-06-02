# Project YALL
This was sort of an interview question, I asked for clarification on the "reference to data" issue but didn't hear back.
Anyway, my main goal was to not have an explicit `new` or `delete` in the code (and not leak). There's an obvious foot gun lying in wait if you use 
a reference type for the container.

## problem statement
```
Create a C++ program that implements a linked list. The linked list should be 
a doubly linked list, allowing movement forward and backward. 
- This program should allow you to add and remove nodes from the list. 
- Each node should contain a reference to application data. 

```

## building
The project uses cmake to build and will fetch [Googletest](https://github.com/google/googletest) from GitHub (internet connection required for the build phase).
Testing is enabled by default, to disable:
```
> mkdir .build && cd .build
> cmake .. -DBUILD_YALL_TESTS=0
```
Otherwise
```
> mkdir .build && cd .build
> cmake .. 
> make -j10
> ctest
> # or
> ./test/yall_test
```

## the reference question
"Each node should contain a reference to application data." The linked-list I wrote does fulfill this requirement if the 
client code specifies that it's using a container of references:
```cpp
yall::Yall<double&> ll_dref;
yall::Yall<double> ll_d;     // node data will contain copies of the client data
```
There are life-time issues when the node data is a reference to client data but all unit-tests use reference types. 

## apps
There are two executables in the `apps` subfolder. The first one, `yall_app1`, uses value data types in the linked list.
The second one, `yall_app2`, uses reference data types (for example, `double&` rather than `double`).
The Google test libraries don't play nice with sanitizer compile flags and the linked-list library is header only.
So these apps are aimed at exercising the API and running executables with the address sanitizer enabled.
This has been tested on windows with MSVC 19.39, and linux with gcc 10.5.
The cmake default os to not build with the address sanitizer enabled. To enable:
``` 
> mkdir .build && cd .build
> cmake .. -DSANITIZE_YALL_APPS=1
> make -j10
> ./apps/yall_app1
>
> ./apps/yall_app2
```
You should run these apps in a terminal to see the proper output (running in an IDE may not show sanitizer output).

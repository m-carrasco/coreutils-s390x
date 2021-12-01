# coreutils-s390x

# How to:

 1.  Create the container, which compiles *coreutils* targeting *s390x* using *wllvm* and *clang-9*: 
     
     ```cd cross-compilation && sudo docker build -t s390x_build_image . && sudo docker create s390x_build_image```

 2. Extract the *coreutils* folder from the container:
 
    ```sudo docker cp $(docker ps -alq):/coreutils-8.31 ./coreutils-8.31```
    
    Binaries and bitcode files are in ```./coreutils-8.31/obj-llvm/src```
    
    Check a binary's architecture: ```file ./coreutils-8.31/obj-llvm/src/ls```
    
 3. Run an *Ubuntu/s390x* container with *Docker* and *QEMU* to execute the binaries:
 
    1. Install the required dependencies: 
     
          ```sudo apt install -y qemu qemu-user-static qemu-user binfmt-support```
    
    2. Run the container and share coreutils' folder:
    
          ```sudo docker run -it --rm -v /usr/bin/qemu-s390x-static:/usr/bin/qemu-s390x-static -v $(pwd)/coreutils-8.31:/coreutils-8.31 s390x/ubuntu:focal```
    
    3. In the container's terminal (*Ubuntu/s390x*), test the binary:
     
          ```/coreutils-8.31/obj-llvm/src/ls --version```


# Remarks:
 
1. Building a Docker image for a different architecture is not straightforward[1], that is why there is no Dockerfile for the runtime step.
2. *wllvm* is hacked to propagate ```--target``` when invoking clang for linking.
3. *factor.c* is disabled because it crashes *clang-9* at the code generation step. *clang-10* failed similarly.
4. To avoid cross-compilation, building coreutils in the *Ubuntu/s390x* was not possible. *clang* (all available clang versions) crashed several times.
 
# References:

1. https://www.ecliptik.com/Cross-Building-and-Running-Multi-Arch-Docker-Images/#qemu-on-linux

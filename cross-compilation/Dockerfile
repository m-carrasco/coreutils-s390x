FROM ubuntu:focal-20211006


##############
# DEPENDENCIES
##############

# Do not use an old Ubuntu version because the available gcc-s390x-linux-gnu has a known bug.
# https://github.com/travitch/whole-program-llvm/issues/75
# In this version (focal-20211006), it is fixed.
RUN apt update && apt install clang-9 llvm-9 gcc-s390x-linux-gnu make git python3 python3-pip wget nano --yes

#######
# WLLVM
#######

COPY ./wllvm.patch /wllvm.patch
# Hack wllvm for cross-compilation targeting s390x.
RUN git clone https://github.com/travitch/whole-program-llvm.git
RUN cat /wllvm.patch
RUN cd whole-program-llvm/ && git checkout c66344231720b4a0780c5bed4219cb5314bbcad5 && git apply /wllvm.patch && pip3 install -e .

#################
# COREUTILS BUILD
#################

# Version used in the tutorial I followed
RUN wget https://ftp.gnu.org/gnu/coreutils/coreutils-8.31.tar.xz
RUN tar xf coreutils-8.31.tar.xz
COPY ./factor.c coreutils-8.31/src/factor.c
RUN cd coreutils-8.31 && mkdir obj-llvm
RUN cd coreutils-8.31/obj-llvm && LLVM_CC_NAME=clang-9 LLVM_LINK_NAME=llvm-link-9 LLVM_AR_NAME=llvm-ar-9 BINUTILS_TARGET_PREFIX=s390x-linux-gnu LLVM_COMPILER=clang CC=wllvm ../configure --host="s390x-linux-gnu" --disable-nls CFLAGS="--target=s390x-linux-gnu -g -O1 -Xclang -disable-llvm-passes -D__NO_STRING_INLINES -D_FORTIFY_SOURCE=0 -U__OPTIMIZE__ -I/usr/s390x-linux-gnu/include/ -L/usr/s390x-linux-gnu/lib"
RUN cd coreutils-8.31/obj-llvm && LLVM_CC_NAME=clang-9 LLVM_LINK_NAME=llvm-link-9 LLVM_AR_NAME=llvm-ar-9 BINUTILS_TARGET_PREFIX=s390x-linux-gnu LLVM_COMPILER=clang make -j4
RUN cd coreutils-8.31/obj-llvm/src && export LLVM_CC_NAME=clang-9 && export LLVM_LINK_NAME=llvm-link-9 && export LLVM_AR_NAME=llvm-ar-9 && export BINUTILS_TARGET_PREFIX=s390x-linux-gnu && export LLVM_COMPILER=clang && find . -executable -type f | xargs -I '{}' extract-bc '{}'
# To output how things are doing
RUN file coreutils-8.31/obj-llvm/src/ls
RUN llvm-dis-9 coreutils-8.31/obj-llvm/src/ls.bc
RUN head --lines=150 coreutils-8.31/obj-llvm/src/ls.ll

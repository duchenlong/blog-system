# shell 		执行一个shell命令, 并将shell命令的结果作为函数的返回
# BUILD_ROOT 	当前的绝对地址
# INCLUDE_PATH 	include 头文件的地址
# BUILD_PATH 	每个需要编译的子目录
# make -C		指定需要编译哪个目录中的 makefile
# all 			遍历执行每个 BUILD_PATH 中的 makefile


export BUILD_ROOT=$(shell pwd)
export INCLUDE_PATH=$(BUILD_ROOT)/include
export BUILD_PATH = $(BUILD_ROOT)/src

all:
	@for dir in $(BUILD_PATH); \
	do \
		make -C $$dir; \
	done 

clean:
	rm -rf $(BUILD_ROOT)/link_obj
	rm -rf $(BUILD_ROOT)/dep
	rm -rf main

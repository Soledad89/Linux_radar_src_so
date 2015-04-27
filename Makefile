#set your project name
PRJ_NAME = libML_GVIDMW.so

#set your project type : choose one below
cXX = g++  -fPIC -shared  -fvisibility=hidden
#cXX = g++ 
#cXX = ar -r 

#set Debug or Release
#Compile_Flag = Debug
Compile_Flag = Release

#set your output path
Output:= bin

#set your source folder
src:= src

#add the lib you used here
#LIBS := -lLib1 -lLib2 -lLib3
LIBS := -lpthread -L/usr/lib 
#LIBPATH := -Lpath1 -Lpath2 -Lpath3
LIBPATH := 
INCLUDEPATH := -I/usr/include
# INCLUDEPATH := -I/usr/lib/XXX/include

###################################
#DON"T MODIFY THE BELOWS

#combine output folder
FinalOutput := $(Output)/$(Compile_Flag)/

#list all dirs
SUBDIRS := $(shell find $(src) -type d)
#flags in makefile
DEBUG_FLAG = -O0 -g3 -Wall -c -fmessage-length=0 #-fvisibility=hidden
RELEASE_FLAG = -O3 -Wall -c -fmessage-length=0 #-fvisibility=hidden
RM := rm -rf

#set compile flag
ifeq ($(Compile_Flag),Debug)
CFLAGS := $(DEBUG_FLAG)
else
CFLAGS := $(RELEASE_FLAG)
endif

#prepare files

CPP_SOURCES = $(wildcard $(src)/*.cpp)
CPP_SOURCES +=$(wildcard $(src)/FrameBuffer/*.cpp) 
CPP_SOURCES +=$(wildcard $(src)/RadarDisplay/*.cpp) 
CPP_SOURCES +=$(wildcard $(src)/Timer/src/*.cpp) 

OBJS:=$(CPP_SOURCES:%.cpp=$(FinalOutput)%.o)

#all target
all: dir $(FinalOutput)$(PRJ_NAME)

dir:
	mkdir -p $(FinalOutput);
	for val in $(SUBDIRS);do \
	  mkdir -p $(FinalOutput)$${val}; \
	done;

$(FinalOutput)%o:./%cpp
	@echo 'Building file: $<'
	@echo 'Invoking:GCC C++ Compiler'
	g++ $(CFLAGS) $(INCLUDEPATH)  -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
#tool invocations

$(FinalOutput)$(PRJ_NAME):$(OBJS)

	@echo 'Building target: $@'
	@echo 'Invoking:GCC C++ Linker'
	$(cXX) $(LIBPATH) -o"$@" $^ $(LIBS)  
	@echo 'Finished building target: $@'
	@echo ' '

#other targets
clean:
	-$(RM) $(Output)/*
	-@echo ' '
.PHONY:all clean
.SECONDARY:


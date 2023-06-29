SHELL := /bin/bash

cxx_source_files := $(shell find src -name *.cpp)
cxx_object_files := $(patsubst src/%.cpp, bin-int/%.cpp.o, $(cxx_source_files))

CXX = x86_64-pc-linux-gnu-g++
LD = x86_64-pc-linux-gnu-g++
CXXFLAGS = -m64 -std=c++20 -Wall -Wextra -Isrc -O2 -fno-exceptions -g
LDFLAGS = -Isrc
TARGET = bin/part-testing

.PHONY: all build clean test-img dependencies mkgpt clean-full

all: clean dependencies test-img build
	@echo --------------
	@echo Build Complete
	@echo --------------

mkgpt:
	@echo --------------
	@echo Building mkgpt
	@echo --------------
	@rm -fr depend/tools/build/mkgpt
	@mkdir -p depend/tools/build/mkgpt
	@curl -OL https://github.com/WorldOS-dev/various-scripts/raw/master/mkgpt/mkgpt.tar.gz
	@tar -xf mkgpt.tar.gz -C depend/tools/build/mkgpt
	@rm -fr mkgpt.tar.gz
	@cd depend/tools/build/mkgpt && ./configure
	@$(MAKE) -C depend/tools/build/mkgpt
	@mkdir -p depend/tools/bin depend/tools/licenses/mkgpt
	@curl -o depend/tools/licenses/mkgpt/LICENSE https://raw.githubusercontent.com/WorldOS-dev/various-scripts/master/mkgpt/LICENSE
	@cp depend/tools/build/mkgpt/mkgpt depend/tools/bin
	@chmod +x depend/tools/bin/mkgpt
	@rm -fr depend/tools/build/mkgpt

dependencies:
	@echo ---------------------
	@echo Fetching dependencies
	@echo ---------------------
ifeq ("$(wildcard depend/tools/bin/mkgpt)","")
	@$(MAKE) mkgpt
endif

build: $(cxx_object_files)
	@mkdir -p bin
	@echo -------
	@echo Linking
	@echo -------
	$(LD) $(cxx_object_files) -o $(TARGET) $(LDFLAGS)

clean:
	@echo --------
	@echo Cleaning
	@echo --------
	@rm -fr bin bin-int image

clean-full:
	@echo ------------
	@echo Cleaning All
	@echo ------------
	@rm -fr bin bin-int image depend

test-img:
	@echo -------------------
	@echo Creating test image
	@echo -------------------
	@mkdir -p image
	@dd if=/dev/zero of=image/fat.img bs=1k count=1440 &>/dev/null
	@mformat -i image/fat.img -f 1440 -I 32 ::
	@./depend/tools/bin/mkgpt -o image/hdimage.bin --image-size 8192 --part image/fat.img --type system

$(cxx_object_files): bin-int/%.cpp.o : src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -o $@ -c $(patsubst bin-int/%.cpp.o, src/%.cpp, $@) $(CXXFLAGS)
	@echo 
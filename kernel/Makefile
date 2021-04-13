# 最終成果物
TARGET = kernel.elf
# object files
OBJS = main.o

# compile options
CXXFLAGS += -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone \
					 -fno-exceptions -fno-rtti -std=c++17
CPPFLAGS += -I/home/akito/ghq/github.com/uchan-nos/osbook/devenv/x86_64-elf/include/c++/v1 \
						-I/home/akito/ghq/github.com/uchan-nos/osbook/devenv/x86_64-elf/include \
						-I/home/akito/ghq/github.com/uchan-nos/osbook/devenv/x86_64-elf/include/freetype2 \
						-I/home/akito/ghq/github.com/tianocore/edk2/MdePkg/Include \
						-I/home/akito/ghq/github.com/tianocore/edk2/MdePkg/Include/X64 \
						-nostdlibinc -D__ELF__ -D_LDBL_EQ_DBL -D_GNU_SOURCE -D_POSIX_TIMERS -DEFIAPI='__attribute__((ms_abi))'

# link options
LDFLAGS += --entry KernelMain -z norelro -z separate-code --image-base 0x100000 --static

# PHONY: 偽物のターゲット, セットした名前と同名ファイルがあったときに便利っぽい
# all: デフォルトのビルド.
.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	rm -rf *.o

# $^: 必須項目をスペース区切りで並べる
# $@: ターゲット(拡張子含む)
kernel.elf: $(OBJS)
	ld.lld-11 $(LDFLAGS) -o $@ ${OBJS}

# $<: 必須項目の先頭ひとつ
%.o: %.cpp Makefile
	clang++-10 $(CPPFLAGS) $(CXXFLAGS) -c $<

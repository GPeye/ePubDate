HEAP_SIZE      = 8388208
STACK_SIZE     = 61800

PRODUCT = ePubDate.pdx

# Locate the SDK
SDK = ${PLAYDATE_SDK_PATH}
ifeq ($(SDK),)
	SDK = $(shell egrep '^\s*SDKRoot' ~/.Playdate/config | head -n 1 | cut -c9-)
endif

ifeq ($(SDK),)
$(error SDK path not found; set ENV value PLAYDATE_SDK_PATH)
endif

######
# IMPORTANT: You must add your source folders to VPATH for make to find them
# ex: VPATH += src1:src2
######

VPATH += src:src/epub:src/unzip

# List C source files here
SRC = src/main.c \
	src/unzip/inffast.c \
	src/unzip/inflate.c \
	src/unzip/inftrees.c \
	src/unzip/unzip.c \
	src/unzip/zutil.c \
	src/unzip/adler32.c \
	src/unzip/crc32.c \
	src/epub/epub.c 

# List all user directories here
UINCDIR = src/unzip:src/epub

# List user asm files
UASRC = 

# List all user C define here, like -D_DEBUG=1
UDEFS = 

# Define ASM defines here
UADEFS = 

# List the user directory to look for the libraries here
ULIBDIR =

# List all user libraries here
ULIBS =

include $(SDK)/C_API/buildsupport/common.mk


cmake_minimum_required(VERSION 3.14)
set(CMAKE_C_STANDARD 11)

set(ENVSDK $ENV{PLAYDATE_SDK_PATH})

if (NOT ${ENVSDK} STREQUAL "")
	# Convert path from Windows
	file(TO_CMAKE_PATH ${ENVSDK} SDK)
else()
	execute_process(
			COMMAND bash -c "egrep '^\\s*SDKRoot' $HOME/.Playdate/config"
			COMMAND head -n 1
			COMMAND cut -c9-
			OUTPUT_VARIABLE SDK
			OUTPUT_STRIP_TRAILING_WHITESPACE
	)
endif()

if (NOT EXISTS ${SDK})
	message(FATAL_ERROR "SDK Path not found; set ENV value PLAYDATE_SDK_PATH")
	return()
endif()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_XCODE_GENERATE_SCHEME TRUE)

# Game Name Customization
set(PLAYDATE_GAME_NAME epubdate)
set(PLAYDATE_GAME_DEVICE epub_date_DEVICE)

project(${PLAYDATE_GAME_NAME} C ASM)

file(GLOB UNZIP_GLOB
     "src/unzip/*.c"
     "src/unzip/*.h"
)

file(GLOB EPUB_GLOB
     "src/epub/*.c"
     "src/epub/*.h"
)

file(GLOB UZ_GLOB
     "src/uz/*.c"
     "src/uz/*.h"
)

include_directories("src/unzip")
include_directories("src/epub")
include_directories("src/uz")

if (TOOLCHAIN STREQUAL "armgcc")
	add_executable(${PLAYDATE_GAME_DEVICE} src/main.c ${UNZIP_GLOB} ${EPUB_GLOB})
else()
	add_library(${PLAYDATE_GAME_NAME} SHARED src/main.c ${UNZIP_GLOB} ${EPUB_GLOB})
endif()

include(${SDK}/C_API/buildsupport/playdate_game.cmake)


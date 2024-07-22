
PLATFORM	?= PLATFORM_WEB
BUILD_MODE	?= DEBUG
FILES		 = main.c line.c section.c game.c player.c
TEST_FILES	 = test.c line.c section.c
CC		 = gcc
CFLAGS		:= -Wall -Wextra -g
INCLUDE_PATHS	:= -Iinclude

ifeq ($(PLATFORM), PLATFORM_WEB)
    CC = emcc
endif

ifeq ($(RAYLIB_BUILD_MODE),RELEASE)
    ifeq ($(PLATFORM),PLATFORM_WEB)
        CFLAGS += -Os
    endif
    ifeq ($(PLATFORM),PLATFORM_DESKTOP)
        CFLAGS += -O1
    endif
    ifeq ($(PLATFORM),PLATFORM_ANDROID)
        CFLAGS += -O2
    endif
endif
ifeq ($(BUILD_MODE), DEBUG)
    CFLAGS += -g -D DEBUG
endif

all: $(FILES)
ifeq ($(PLATFORM), PLATFORM_WEB)
	mkdir --parents ../out/web
	$(CC) $(FILES) -o ../out/web/main.html $(CFLAGS) -Os ./lib/web/libraylib.a -I. -Iinclude -s USE_GLFW=3 --shell-file index.html -DPLATFORM_WEB --preload-file resources/starry_night.png --preload-file resources/spaceship.png -s FORCE_FILESYSTEM=1 -s ALLOW_MEMORY_GROWTH=1
endif
ifeq ($(PLATFORM), PLATFORM_DESKTOP)
	mkdir --parents ../out/desktop
	$(CC) $(FILES) -o ../out/desktop/main $(CFLAGS) -Llib/desktop/ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

test: $(TEST_FILES)
	mkdir --parents ../out/test
	gcc $(TEST_FILES) -o ../out/test/main

clean:
	rm -dr ../out/*
	rm -d ../out


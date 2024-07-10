
PLATFORM	?= PLATFORM_WEB
FILES		 = main.c wall.c section.c
CC		 = gcc
CFLAGS		:= -Wall -Wextra -g
INCLUDE_PATHS	:= -Iinclude

ifeq ($(PLATFORM), PLATFORM_WEB)
    CC = emcc
endif

all: $(FILES)
ifeq ($(PLATFORM), PLATFORM_WEB)
	mkdir --parents ../out/web
	$(CC) $(FILES) -o ../out/web/main.html $(CFLAGS) -Os ./lib/web/libraylib.a -I. -Iinclude -s USE_GLFW=3 --shell-file index.html -DPLATFORM_WEB
endif
ifeq ($(PLATFORM), PLATFORM_DESKTOP)
	mkdir --parents ../out/desktop
	$(CC) $(FILES) -o ../out/desktop/main $(CFLAGS) -Llib/desktop/ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

clean:
	rm -dr ../out/*
	rm -d ../out


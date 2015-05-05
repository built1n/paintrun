CC = clang

PLATFORM = sdl

SRC := src/*.c
OBJ := $(SRC:.c=.o)

CFLAGS = -Isrc/ -O3 -std=c99 -g -Wall -fsanitize=address

PLATSRC := platforms/$(PLATFORM)/*.c
PLATOBJ = $(PLATSRC:.c=.o)

HEADERS := platforms/$(PLATFORM)/*.h src/*.h

include platforms/$(PLATFORM)/*.make

paintrun-sdl: $(OBJ) $(PLATOBJ) Makefile $(HEADERS)
	@echo "LD $@"
	@$(CC) $(OBJ) $(PLATOBJ) -o $@ $(CFLAGS) $(PLATFLAGS)

%.o: %.c Makefile $(HEADERS)
	@echo "CC $<"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning build directory..."
	@rm -f $(OBJ) $(PLATOBJ) paintrun-sdl

all:
	@echo $(PLATSRC) $(PLATOBJ) $(SRC) $(OBJ)

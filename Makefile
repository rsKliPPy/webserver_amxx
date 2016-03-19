# Configure these for yourself
HLSDK = /media/klippy/E44CECDF4CECAD8C/Users/KliPPy/Documents/AMXXModules/hlsdk
METAMOD = /media/klippy/E44CECDF4CECAD8C/Users/KliPPy/Documents/AMXXModules/metamod-hl1/metamod
AMTL = /media/klippy/E44CECDF4CECAD8C/Users/KliPPy/Documents/AMXXModules/amtl

# Compiler of choice
CC = gcc

#------------------------------------------------------------------------------

PROJECT = webserver

OBJECTS = main.o natives.o amxxsdk/amxxmodule.o libmicrohttpd.a

INCLUDES = -I. -Iamxxsdk/ -I$(HLSDK)/common -I$(HLSDK)/dlls -I$(HLSDK)/engine -I$(HLSDK)/pm_shared -I$(HLSDK)/public \
	-I$(METAMOD) -I$(AMTL) -Ilibmicrohttpd/include

LIBS = -lmicrohttpd

OPTS = -std=c++11 -shared -fpic -static -fno-exceptions -fno-rtti -DHAVE_STDINT_H -fno-strict-aliasing -m32 -Wall -Werror \
	-Wno-error=unused-result

#------------------------------------------------------------------------------

OUTPUT = $(PROJECT)_amxx_i386.so


all: $(PROJECT)

libmicrohttpd.a: libmicrohttpd.so
	ar rcs $@ $<

%.o: %.cpp
	$(CC) -c $(OPTS) $(INCLUDES) $< -o $@ $(LIBS)

$(PROJECT): $(OBJECTS)
	$(CC) $(OPTS) $(INCLUDES) -o $(OUTPUT) $^ $(LIBS)

clean:
	rm -f *.o
	rm -f amxxsdk/*.o


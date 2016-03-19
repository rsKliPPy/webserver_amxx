#------------------------------------------------------------------------------

HTTPS_SUPPORT = false

# Compiler of choice
CC=gcc

#------------------------------------------------------------------------------

PROJECT = libmicrohttpd

INCLUDES = -I./microhttpd -I./include

LIBS = 

OPTS = -shared -m32 -Wall -Werror -Wno-error=unused-result -Wno-error=unused-variable

SOURCES = md5.cpp base64.cpp daemon.cpp tsearch.cpp internal.cpp response.cpp basicauth.cpp connection.cpp digestauth.cpp \
	memorypool.cpp sysfdsetsize.cpp postprocessor.cpp reason_phrase.cpp mhd_mono_clock.cpp

ifeq ($(HTTPS_SUPPORT),true)
 SOURCES += connection_https.c
 LIBS += -lgnutls
 OPTS += -DHTTPS_SUPPORT
endif

SOURCESDIR = ./microhttpd/
OUTDIR = ..

#------------------------------------------------------------------------------

OUTPUT = $(OUTDIR)/$(PROJECT).a
SFILES = $(addprefix $(SOURCESDIR), $(SOURCES))
OBJECTS = $(subst .cpp,.o, $(SFILES))


all: $(PROJECT)

%.o: %.c
	$(CC) -c $(OPTS) $(INCLUDES) -o $@ $< $(LIBS)

$(PROJECT): $(OBJECTS)
	ar rcs $(OUTPUT) $(OBJECTS)

clean:
	rm -f *.o
	rm -f $(SOURCESDIR)/*.o






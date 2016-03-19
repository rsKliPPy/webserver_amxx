#------------------------------------------------------------------------------

# To supports HTTPS, you also have to go into MHD_config.h
# and define SUPPORT_HTTPS in there
SUPPORT_HTTPS = false

# Compiler of choice
CC=gcc

#------------------------------------------------------------------------------

PROJECT = libmicrohttpd

INCLUDES = -I./microhttpd -I./include

LIBS = 

OPTS = -shared -m32 -Wall -Werror -Wno-error=unused-result

SOURCES = md5.cpp base64.cpp daemon.cpp tsearch.cpp internal.cpp response.cpp basicauth.cpp connection.cpp digestauth.cpp \
	memorypool.cpp sysfdsetsize.cpp postprocessor.cpp reason_phrase.cpp mhd_mono_clock.cpp

ifeq ($(SUPPORT_HTTPS),true)
 SOURCES += connection_https.c
 LIBS += -lgnutls
endif

SOURCESDIR = ./microhttpd/
OUTDIR = ..

#------------------------------------------------------------------------------

OUTPUT = $(OUTDIR)/$(PROJECT).so
SFILES = $(addprefix $(SOURCESDIR), $(SOURCES))
OBJECTS = $(subst .cpp,.o, $(SFILES))


$(PROJECT): $(OBJECTS)
	$(CC) $(OPTS) $(INCLUDES) -o $(OUTPUT) $^ $(LIBS)

%.o: %.c
	$(CC) -c $(OPTS) $(INCLUDES) -o $@ $< $(LIBS)

clean:
	rm -f *.o
	rm -f $(SOURCESDIR)/*.o


all: $(PROJECT)




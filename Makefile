export CPATH = /usr/include/libxml2

LDLIBS = -lxml2

BIN = 2csv 2xml csv2 xml2

all: $(BIN)

clean: ; $(RM) $(BIN)

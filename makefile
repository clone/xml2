VERSION=0.1

FILES=xml2 html2 2html 2xml
TAR=xml2-$(VERSION).tar.gz
DIR=xml2-$(VERSION)

CFLAGS=-g -Wall `xml-config --cflags`
# CFLAGS=-O2 `xml-config --cflags`
LDLIBS=`xml-config --libs`

all: $(FILES)

xml2: xml2.c
html2:
	ln -sf xml2 html2

2xml: 2xml.c
2html:
	ln -sf 2xml 2html

clean:
	$(RM) *.o $(FILES) $(TAR) $(DIR)

tar: clean
	ln -s . $(DIR)
	tar cvfz $(TAR) $(DIR)/*

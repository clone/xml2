VERSION=0.2

FILES=xml2 html2 2html 2xml csv2
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

csv2: csv2.c

clean:
	$(RM) *.o $(FILES) xml2-*

tar: clean
	ln -s . $(DIR)
	tar cvfz $(TAR) $(DIR)/*

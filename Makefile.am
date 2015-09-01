AM_CPPFLAGS = $(XML_CFLAGS)
bin_PROGRAMS = xml2 2xml csv2 2csv
xml2_SOURCES = xml2.c
xml2_LDADD = -lxml
2xml_SOURCES = 2xml.c
2xml_LDADD = -lxml
csv2_SOURCES = csv2.c
2csv_SOURCES = 2csv.c

install-exec-hook:
	ln -sf $(DESTDIR)$(bindir)/xml2 $(DESTDIR)$(bindir)/html2
	ln -sf $(DESTDIR)$(bindir)/2xml $(DESTDIR)$(bindir)/2html

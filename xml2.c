/*
    xml2 -- convert XML to line-oriented file format.
    Copyright (C) 2000  Daniel Egnor

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <libxml/parser.h>
#include <libxml/HTMLparser.h>

struct node
{
	char *name;
	struct node *parent;
};

int do_compress_whitespace = 0;
int do_ignore_whitespace = 0;
int do_trim_whitespace = 0;

struct node *path = NULL;
char *last = NULL;
int was_output = 0;

char *content_buffer = NULL;
int content_len = 0,content_alloc = 0;

static void write_path_trailing(struct node *p)
{
	if (NULL != p) {
		write_path_trailing(p->parent);
		fputs(p->name,stdout);
	}
	putchar('/');

	was_output = 1;
	if (NULL != last) {
		free(last);
		last = NULL;
	}
}

static void write_path(struct node *p)
{
	if (NULL == p) putchar('/');
	else {
		write_path_trailing(p->parent);
		fputs(p->name,stdout);
	}
}

static void flush_content(void)
{
	const char *begin = content_buffer,*end = content_buffer + content_len;
	content_len = 0;

	if (do_trim_whitespace) {
		while (end != begin && isspace(begin[0])) ++begin;
		while (end != begin && isspace(end[-1])) --end;
	}

	if (end == begin) return;

	if (do_ignore_whitespace) {
		const char *ch = begin;
		while (end != ch && isspace(*ch)) ++ch;
		if (end == ch) return;
	}

	if (do_compress_whitespace) {
		write_path(path);
		putchar('=');
		do {
			if (isspace(*begin)) {
				do ++begin; 
				while (begin != end && isspace(*begin));
				putchar(' ');
			}

			if (begin != end) do putchar(*begin++);
			while (begin != end && !isspace(*begin));
		} while (begin != end);
		putchar('\n');
	} else {
		const char *nl;

		while (NULL != (nl = memchr(begin,'\n',end - begin))) {
			write_path(path);
			putchar('=');
			do putchar(*begin); while (begin++ != nl);
		}

		write_path(path);
		putchar('=');
		while (begin != end) putchar(*begin++);
		putchar('\n');
	}
}

static void enter(const xmlChar *name,int attr)
{
	struct node *leaf = malloc(sizeof(struct node));
	leaf->parent = path;
	leaf->name = NULL;

	flush_content();
	path = leaf;
	was_output = 0;

	if (!attr && NULL != last && !strcmp(name,last)) {
		leaf->name = last;
		last = NULL;
		write_path(path);
		fputs("\n",stdout);
	}

	if (NULL != last) free(last);
	last = NULL;

	if (NULL == leaf->name) {
		leaf->name = malloc(2 + strlen(name));
		strcpy(leaf->name,attr ? "@" : "");
		strcat(leaf->name,name);
	}
}

static void leave(void)
{
	struct node *leaf = path;
	flush_content();
	if (!was_output) {
		write_path(path);
		putchar('\n');
	}

	if (NULL != last) free(last);
	path = leaf->parent;
	last = leaf->name;
	free(leaf);
}

static void content(const char *ch,int len,int strip)
{
	if (content_len + len > content_alloc) {
		content_alloc = 2*(content_len + len);
		content_buffer = realloc(content_buffer,content_alloc);
	}

	memcpy(content_buffer + content_len,ch,len);
	content_len += len;
}

static void sax_characters(void *x,const xmlChar *ch,int len)
{
	content(ch,len,1);
}

static void sax_start_element(void *x,const xmlChar *name,const xmlChar **atts)
{
	enter(name,0);
	while (NULL != atts && NULL != *atts) {
		enter(atts[0],1);
		content(atts[1],atts[1] ? strlen(atts[1]) : 0,0);
		leave();
		atts += 2;
	}
}

static void sax_end_element(void *x,const xmlChar *name)
{
	leave();
}

static void sax_pi(void *x,const xmlChar *target,const xmlChar *data)
{
	write_path_trailing(path);
	putchar('?');
	fputs(target,stdout);
	putchar('=');
	fputs(data,stdout);
	putchar('\n');
}

static void sax_comment(void *x,const xmlChar *value)
{
	enter("!",0);
	content(value,strlen(value),1);
	leave();
}

static void sax_error(void *x,const char *msg,...)
{
	va_list ap;
	va_start(ap,msg);
	fputs("error: ",stderr);
	vfprintf(stderr,msg,ap);
	va_end(ap);
}

static void init(xmlSAXHandlerPtr sax)
{
	sax->startElement = sax_start_element;
	sax->endElement = sax_end_element;
	sax->characters = sax_characters;
	sax->processingInstruction = sax_pi;
	sax->comment = sax_comment;
	sax->warning = sax_error;
	sax->error = sax_error;
	sax->fatalError = sax_error;
}

int main(int argc,char *argv[])
{
	xmlSAXHandler sax;
	xmlParserCtxtPtr ctxt;
	const char *name;
	char buffer[4096];
	int num;

	int (*parseChunk)(xmlParserCtxtPtr,const char *,int,int);
	void (*freeCtxt)(xmlParserCtxtPtr);

	name = strrchr(argv[0],'/');
	if (NULL == name) name = argv[0]; else ++name;

	xmlSubstituteEntitiesDefault(1);
	memset(&sax,'\0',sizeof(sax));
	init(&sax);

	if (1 == argc && !strcmp(name,"html2")) {
		ctxt = htmlCreatePushParserCtxt(&sax,NULL,NULL,0,"stdin",0);
		parseChunk = htmlParseChunk;
		freeCtxt = htmlFreeParserCtxt;
		do_compress_whitespace = 1;
	} else if (1 == argc && !strcmp(name,"xml2")) {
		ctxt = xmlCreatePushParserCtxt(&sax,NULL,NULL,0,"stdin");
		parseChunk = xmlParseChunk;
		freeCtxt = xmlFreeParserCtxt;
		do_ignore_whitespace = 1;
	} else {
		fputs("usage: [xml2|html2] < [xml|html] > out\n",stderr);
		return 2;
	}

	while (0 < (num = read(0,buffer,sizeof(buffer)))
	   &&  0 == parseChunk(ctxt,buffer,num,0)) ;
	parseChunk(ctxt,NULL,0,1);

	freeCtxt(ctxt);
	return (0 != num);
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

int do_first = 0;
const char *quote = "";
const char *delimiter = ",";

int num_fields = 0;
int recno = 0;
const char **field_names = NULL;

int isvalid(const char *begin,const char *end) {
	return begin != end && isalnum(*begin);
}

void field(int num,const char *begin,const char *end) {
	while (begin != end && isspace(begin[0])) ++begin;
	while (end != begin && isspace(end[-1])) --end;

	if (do_first) {
		const char *name;
		char *ptr;

		if (num >= num_fields)
			field_names = realloc(field_names,
				sizeof(*field_names) * (1 + num));

		while (num >= num_fields)
			field_names[num_fields++] = NULL;

		name = ptr = malloc(7 + end - begin);

		if (isvalid(begin,end))
			do *ptr++ = *begin++;
			while (isvalid(begin,end));
		else {
			strcpy(ptr,"field");
			ptr += 5;
		}

		while (end != begin) {
			while (end != begin && !isvalid(begin,end)) ++begin;
			if (isvalid(begin,end)) {
				*ptr++ = '-';
				do *ptr++ = *begin++;
				while (isvalid(begin,end));
			}
		}

		*ptr++ = '\0';
		field_names[num] = name;
	} else {
		fputs("/file/record/",stdout);
		if (num < num_fields && NULL != field_names[num])
			fputs(field_names[num],stdout);
		else
			printf("field%d",num);
		printf("=%.*s\n",end - begin,begin);
	}
}

void line(const char *l) {
	int num = 0;
	if (!do_first) {
		fputs("/file/record\n",stdout);
		printf("/file/record/@num=%d\n",recno++);
	}
	for (;;) {
		if (NULL != strchr(quote,*l)) {
			size_t len = strcspn(++l,quote);
			field(num++,l,len + l);
			l += len;
			if (NULL != strchr(quote,*l)) ++l;
		} else {
			size_t len = strcspn(l,delimiter);
			field(num++,l,len + l);
			l += len;
		}

		if ('\0' == *l) break;
		if (NULL != strchr(delimiter,*l)) ++l;
	}
	do_first = 0;
}

int main(int argc,char *argv[]) {
	int arg,num,alloc,len = 0;
	char *buffer = malloc(alloc = 4096);

	while (EOF != (arg = getopt(argc,argv,"fq:d:"))) switch (arg) {
	case 'f': ++do_first; break;
	case 'q': quote = optarg; break;
	case 'd': delimiter = optarg; break;
	case '?':
		fputs("usage: csv2 [-f] [-q quote] [-d comma] < csv > out\n",
		      stderr);
		return 2;
	}

	if (optind != argc) {
		fprintf(stderr,"unexpected argument: \"%s\"\n",argv[optind]);
		return 2;
	}

	while ((num = read(0,len + buffer,alloc - len)) > 0) {
		char *end = buffer + len + num,*ptr = buffer,*eol;
		while ((eol = memchr(ptr,'\n',end - ptr))) {
			*eol++ = '\0';
			line(ptr);
			ptr = eol;
		}

		memmove(buffer,ptr,(len = end - ptr));
		if (len == alloc) buffer = realloc(buffer,alloc *= 2);
	}

	if (len > 0) {
		buffer[len] = '\0';
		line(buffer);
	}
	/* free(buffer); */
	return 0;
}

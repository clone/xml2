#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void usage(void) {
	fputs("usage: 2csv [-q quote] [-d comma] "
	      "record field [field ...] < in > csv\n",stderr);
	exit(2);
}

void dump(int num,char **vec,const char *q,const char *d) {
	int i;
	for (i = 0; i < num; ++i) if (NULL != vec[i]) break;
	if (num == i) return;
	for (i = 0; i < num; ++i) {
		if (i > 0) fputs(d,stdout);
		if (NULL != vec[i]) {
			if (strcspn(vec[i],d) == strlen(vec[i])) 
				fputs(vec[i],stdout);
			else {
				fputs(q,stdout);
				fputs(vec[i],stdout);
				fputs(q,stdout);
			}

			free(vec[i]);
			vec[i] = NULL;
		}
	}
	fputs("\n",stdout);
}

int main(int argc,char *argv[]) {
	int i,arg,alloc,len = 0,record_len,*field_len;
	char *buffer = malloc(alloc = 4096),**fields;
	const char *record,*quote = "\"",*delimiter = ",";

	while (EOF != (arg = getopt(argc,argv,"q:d:"))) switch (arg) {
	case 'q': quote = optarg; break;
	case 'd': delimiter = optarg; break;
	case '?': usage();
	}

	if (argc - optind < 2) usage();

	argv += optind; argc -= optind;
	record_len = strlen((record = argv[0]));
	++argv; --argc;

	fields = malloc(argc * sizeof(*fields));
	field_len = malloc(argc * sizeof(*field_len));
	for (i = 0; i < argc; ++i) {
		field_len[i] = strlen(argv[i]);
		fields[i] = NULL;
	}

	while (NULL != fgets(buffer + len,alloc - len,stdin)) {
		char *eq,*find;

		if ('\0' == buffer[len]) break;
		len += strlen(buffer + len);
		if ('\n' != buffer[len - 1]) {
			buffer = realloc(buffer,alloc *= 2);
			continue;
		}

		buffer[--len] = '\0';
		eq = strchr(buffer,'=');
		if (NULL != eq) {
			len = eq - buffer;
			*eq++ = '\0';
		}

		find = strstr(buffer,record);
		if (NULL == find || '/' != find[record_len]
		|| (buffer != find && '/' != find[-1])) {
			dump(argc,fields,quote,delimiter);
			continue;
		}

		for (i = 0; i < argc; ++i) {
			if (field_len[i] <= len
			&& !strcmp(buffer + len - field_len[i],argv[i])
			&& (field_len[i] == len 
			||  '/' == buffer[len - field_len[i] - 1]))
				fields[i] = strdup(eq ? eq : "");
		}

		len = 0;
	}

	dump(argc,fields,quote,delimiter);
	return 0;
}

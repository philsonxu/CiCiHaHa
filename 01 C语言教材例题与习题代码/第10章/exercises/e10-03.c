#include <stdio.h>
#include <stdlib.h>

int main() {
	FILE* fp = NULL;
	char c;
	//int i;

	if ((fp = fopen("test", "w")) == NULL) {
		printf("open file test error!\n");
		exit(EXIT_FAILURE);
	}

	while ((c = getchar()) != EOF && c != '!') {
		if (c >= 'a' && c <= 'z')
			c = c - 'a' + 'A';
		fputc(c, fp);
	}

	fclose(fp);
}


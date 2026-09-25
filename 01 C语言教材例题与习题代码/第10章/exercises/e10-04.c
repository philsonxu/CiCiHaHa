#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(char* s, int i, int j) {
	char t = s[i];
	s[i] = s[j];
	s[j] = t;
}

void select_sort(char* str) {
	int i, j;
	int len = strlen(str);
	for (i = 0; i < len; i++) {
		int min = i;
		for (j = i + 1; j < len; j++) {
			if (str[j] < str[min])
				min = j;
		}
		swap(str, min, i);
	}
}

int main() {
	FILE* fa, * fb, * fc;
	char buf[1024] = { 0 };

	fa = fopen("A", "r");
	fb = fopen("B", "r");
	fc = fopen("C", "w");

	fgets(buf, 1024, fa);
	int len = strlen(buf);
	fgets(buf + len, 1024 - len, fb);
	select_sort(buf);
	fputs(buf, fc);

	fclose(fa);
	fclose(fb);
	fclose(fc);
}


#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE*fp = NULL;
	fp = fopen("./test.txt", "w+");
	fprintf(fp, "lalala...\n");
	fputs("i am singing ...\n",fp);
	fclose(fp);
}

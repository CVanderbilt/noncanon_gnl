#include "line_edition.h"
#include <unistd.h>

int hook(void* data, char*ln)
{
	char *tmp;

	tmp = (char*)data;
	write(1, "data:", 5);
	for (int i = 0; tmp[i]; i++)
		write(1, tmp + i, 1);
	write(1, "\nline:", 6);
	for (int i = 0; ln[i]; i++)
		write(1, ln + i, 1);
	write(1, "\n", 1);
	if (ln[0] == 'q')
		return (0);
	return (1);
}

int main(void)
{
	const char* data = "data1234";
	line_edition_loop((void*)data, "prompt", hook);
}
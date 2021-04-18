#include "utils.h"
#include <unistd.h>
#include <stdlib.h>

int ft_putchar0(int c)
{
	return (write(0, &c, 1));
}

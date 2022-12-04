#include "misc.h"

int FindCharacter(char* string, int size, char delimiter, int instance)
{
	int rc = -1;
	if (instance < 1 || instance > size)
	{
		return -1;
	}

	// Iterate through the string
	for (int i = 0; i < size; i++)
	{
		if (*(string + i) == delimiter)
		{
			instance--;
			if (instance == 0)
			{
				rc = i;
				break;
			}
		}
	}
	return rc;
}
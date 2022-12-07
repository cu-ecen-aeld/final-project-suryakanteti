/**
* Author: Surya Kanteti
*/

#include "misc.h"

/**
 * @brief: Function to find the n'th instance of a character in a fixed size string
 *
 * @param string: The string to be searched
 * @param size: Size of the string to be searched
 * @param delimiter: Charcter to be found
 * @param instance: Instance number of the delimiter to be found
 *
 * @return Integer indicating the position of the delimiter, -1 if not found
 * 
*/
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
/**
* Author: Surya Kanteti
*/

#ifndef __MISC_H__
#define __MISC_H__


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
int FindCharacter(char* string, int size, char delimiter, int instance);

#endif // !__MISC_H__

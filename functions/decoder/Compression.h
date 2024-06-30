//This function compresses files using the LZ77 method

//The function requires the string.h and stdlib.h libraries to work

//The first argument is the address of the file to be compressed
//The second argument is the address of the file where the compressed content should be placed

//The function is designed to work with files up to 150 megabytes in size.If the file size exceeds 
// 150 megabytes, the file must be read in 150 megabyte chunks

//If the work completes successfully, the function returns 1


/*if the character has not been encountered before, it is placed in the structure. 
If a symbol has previously been encountered, the distance to its previous occurrence 
and the number of repeated symbols are recorded in the structure. the function only 
considers the 255 preceding characters. Repeated combinations of less than 4 characters 
are not compressed due to impracticality*/
typedef struct wordC
{
	unsigned char offset;
	unsigned char count;
	unsigned char s;
};

/*a string containing the content to be encoded*/
char originalString[210000000] = "";

int compression(char* in, char* out)
{
	FILE* input = fopen(in, "r");
	FILE* output = fopen(out, "wb");
	/*The array size is immediately set for the maximum possible number of elements in the file. 
	This improves encoder performance, but increases memory usage*/
	wordC* arrayWord = (wordC*)malloc(2100000000 * sizeof(wordC));
	int countWrites = 0;
	fseek(input, 0, SEEK_END);
	int len = ftell(input);
	fseek(input, 0, SEEK_SET);
	char current[256] = "";
	int indexAssistStringCode = 0;
	int indexCurrent = 0;
	int indexOriginalString = 0;
	fgets(originalString, 210000000, input);
	int countWritten = 0;
	char buff;
	char assistStringCode[110000] = "";
	char assistStr[200000] = "";
	/*we write information about each character into an array of structures*/
	while (countWritten < len && indexOriginalString < len)
	{
		/*If the number of characters in the assistStringCode string becomes too large, 
		the encoder's performance will decrease. To avoid this, the asisstStringCode is
		periodically cleared*/
		if (indexAssistStringCode >= 100000)
		{
			strcpy(assistStr, assistStringCode + 50000);
			memset(assistStringCode, '\0', 110000);
			strcpy(assistStringCode, assistStr);
			indexAssistStringCode -= 50000;
		}
		current[indexCurrent] = originalString[indexOriginalString];
		if (indexAssistStringCode <= 255)
		{
			if (strstr(assistStringCode, current) == NULL)
			{
				assistStringCode[indexAssistStringCode] = current[indexCurrent];
				indexAssistStringCode++;
				current[indexCurrent] = '\0';
				countWritten++;
				indexOriginalString++;
				arrayWord[countWrites].offset = 0;
				arrayWord[countWrites].count = 0;
				arrayWord[countWrites].s = assistStringCode[indexAssistStringCode - 1];
				countWrites += 1;
			}
			else
			{
				while (strstr(assistStringCode, current) != NULL && indexOriginalString < len)
				{
					indexCurrent++;
					indexOriginalString++;
					current[indexCurrent] = originalString[indexOriginalString];
					if (indexOriginalString == len)
					{
						break;
					}
				}
				indexOriginalString -= strlen(current) - 1;
				current[indexCurrent] = '\0';
				indexCurrent--;
				if (strlen(current) < 4)
				{
					for (int i = 0; i < strlen(current); i++)
					{
						assistStringCode[indexAssistStringCode] = current[i];
						indexAssistStringCode++;
						countWritten++;
						indexOriginalString++;
						arrayWord[countWrites].offset = 0;
						arrayWord[countWrites].count = 0;
						arrayWord[countWrites].s = assistStringCode[indexAssistStringCode - 1];
						countWrites += 1;
					}
					indexCurrent = 0;
					memset(current, '\0', 5);
				}
				else
				{
					int of = 0;
					while (of <= 255)
					{
						if (strstr(assistStringCode + indexAssistStringCode - of, current) == NULL)
						{
							of++;
						}
						else
						{
							arrayWord[countWrites].offset = of;
							arrayWord[countWrites].count = strlen(current);
							arrayWord[countWrites].s = '\0';
							countWrites += 1;
							int l = strlen(current);
							countWritten += l;
							strcat(assistStringCode, current);
							indexAssistStringCode += l;
							memset(current, '\0', l);
							indexCurrent = 0;
							indexOriginalString += l;
							break;
						}
					}
				}
			}
		}
		else
		{
			if (strstr(assistStringCode + indexAssistStringCode - 255, current) == NULL)
			{
				assistStringCode[indexAssistStringCode] = current[indexCurrent];
				indexAssistStringCode++;
				current[indexCurrent] = '\0';
				countWritten++;
				indexOriginalString++;
				arrayWord[countWrites].offset = 0;
				arrayWord[countWrites].count = 0;
				arrayWord[countWrites].s = assistStringCode[indexAssistStringCode - 1];
				countWrites += 1;
			}
			else
			{
				while (strstr(assistStringCode + indexAssistStringCode - 255, current) != NULL && indexOriginalString < len)
				{
					indexCurrent++;
					indexOriginalString++;
					current[indexCurrent] = originalString[indexOriginalString];
				}
				indexOriginalString -= strlen(current) - 1;
				current[indexCurrent] = '\0';
				indexCurrent--;
				if (strlen(current) < 4)
				{
					for (int i = 0; i < strlen(current); i++)
					{
						assistStringCode[indexAssistStringCode] = current[i];
						indexAssistStringCode++;
						countWritten++;
						indexOriginalString++;
						arrayWord[countWrites].offset = 0;
						arrayWord[countWrites].count = 0;
						arrayWord[countWrites].s = assistStringCode[indexAssistStringCode - 1];
						countWrites += 1;
					}
					indexCurrent = 0;
					memset(current, '\0', 5);
				}
				else
				{
					int of = 0;
					while (of <= 255)
					{
						if (strstr(assistStringCode + indexAssistStringCode - of, current) == NULL)
						{
							of++;
						}
						else
						{
							arrayWord[countWrites].offset = of;
							arrayWord[countWrites].count = strlen(current);
							arrayWord[countWrites].s = '\0';
							countWrites += 1;
							int l = strlen(current);
							countWritten += l;
							strcat(assistStringCode, current);
							indexAssistStringCode += l;
							memset(current, '\0', l);
							indexCurrent = 0;
							indexOriginalString += l;
							break;
						}
					}
				}
			}
		}
	}
	/*write the contents of the structure array to the file byte by byte*/
	for (int i = 0; i < countWrites; i++)
	{
		if (arrayWord[i].offset == 0)
		{
			fputc(arrayWord[i].s, output);
		}
		else
		{
			fputc('|', output);
			fputc(arrayWord[i].offset, output);
			fputc(arrayWord[i].count, output);
		}
	}
	fclose(input);
	fclose(output);
	return 1;
}

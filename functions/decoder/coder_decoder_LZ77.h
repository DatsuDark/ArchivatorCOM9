/*a structure into which information about the symbol will be written
(either the symbol or a link to the previous use of the symbol)*/
typedef struct word
{
	int offset;
	int count;
	char s;
};

/*LZ77 encoding function*/
/*The coder function receives a file and an array of structures as input. 
An array of structures is created in advance*/
void coderLZ77(FILE* input, word* arrayWord)
{
	/*string with all characters*/
	char string[100000] = "";
	/*set the pointer to the beginning of the file*/
	fseek(input, 0, 0);
	fgets(string, 100000, input);
	int numberStruct = 0;
	int countWritten = 0;
	/*a line into which already encoded elements are placed*/
	char assist[100000] = "";
	/*a line that contains the characters we are currently working with*/
	char current[100000] = "";
	int indexString = 0;
	int indexCurrent = 0;
	int indexAssist = 0;
	int stop = 0;
	/*record information about all symbols*/
	while (countWritten < strlen(string))
	{
		current[indexCurrent] = string[indexString];
		/*if the symbol is single and has not been seen before, write it down*/
		if (strstr(assist, current) == NULL)
		{
			assist[indexAssist] = current[indexCurrent];
			indexAssist++;
			current[indexCurrent] = '\0';
			countWritten++;
			indexString++;
			arrayWord[numberStruct].offset = 0;
			arrayWord[numberStruct].count = 0;
			arrayWord[numberStruct].s = assist[indexAssist - 1];
			numberStruct++;
		}
		/*in this block we look for repeated characters and groups of repeated characters*/
		else
		{
			/*Let's assume that we have the combination "ABCCCCC" and at the current
			encoding stage we have "ABC" written. If we now take the combination "CCCC", 
			it will not be found in the line. However, we can encode it as <1, 4>.
			To implement this functionality, you need auxiliary variables "repeat" and
			"isRepeated"*/
			int repeat = 1;
			int isRepeated = 1;
			while (repeat == 1 || strstr(assist, current) != NULL && indexString <= strlen(string) - 1)
			{
				indexCurrent++;
				indexString++;
				current[indexCurrent] = string[indexString];
				if (indexCurrent > 0)
				{
					if (current[indexCurrent] != current[indexCurrent - 1])
					{
						repeat = 0;
					}
				}
			}
			current[indexCurrent] = '\0';
			indexCurrent--;
			int assistIndexCurrent = indexCurrent;
			int assistIndexAssist = indexAssist - 1;
			for (int c = 0; c < strlen(current) - 1; c++)
			{
				for (int v = c + 1; v < strlen(current); v++)
				{
					if (current[c] != current[v])
					{
						isRepeated = 0;
						break;
					}
				}
			}
			if (isRepeated == 1)
			{
				while (isRepeated == 1)
				{
					if (current[assistIndexCurrent] == assist[assistIndexAssist] && assistIndexAssist == indexAssist - 1)
					{
						arrayWord[numberStruct].offset = indexAssist - assistIndexAssist;
						arrayWord[numberStruct].count = strlen(current);
						numberStruct++;
						for (int z = 0; z < strlen(current); z++)
						{
							assist[indexAssist] = current[z];
							countWritten++;
							indexAssist++;
						}
						for (int numberStruct = 0; numberStruct < 100; numberStruct++)
						{
							current[numberStruct] = '\0';
						}
						indexCurrent = 0;
						isRepeated = 0;
					}
					if (current[assistIndexCurrent] == assist[assistIndexAssist] && assistIndexAssist != indexAssist)
					{
						arrayWord[numberStruct].offset = indexAssist - assistIndexAssist;
						arrayWord[numberStruct].count = 1;
						numberStruct++;
						indexString -= strlen(current) - 1;
						assist[indexAssist] = current[0];
						countWritten++;
						indexAssist++;
						for (int numberStruct = 0; numberStruct < 100; numberStruct++)
						{
							current[numberStruct] = '\0';
						}
						indexCurrent = 0;
						isRepeated = 0;
					}
					else
					{
						assistIndexAssist--;
					}
				}
			}
			else
			{
				while (assistIndexCurrent >= 0)
				{
					if (current[assistIndexCurrent] == assist[assistIndexAssist])
					{
						assistIndexCurrent--;
						assistIndexAssist--;
					}
					if (assistIndexCurrent < 0)
					{
						arrayWord[numberStruct].offset = indexAssist - (assistIndexAssist + 1);
						arrayWord[numberStruct].count = strlen(current);
						numberStruct++;
						for (int z = 0; z < strlen(current); z++)
						{
							assist[indexAssist] = current[z];
							countWritten++;
							indexAssist++;
						}
						for (int numberStruct = 0; numberStruct < 100; numberStruct++)
						{
							current[numberStruct] = '\0';
						}
						indexCurrent = 0;
					}
					if (current[assistIndexCurrent] != assist[assistIndexAssist])
					{
						assistIndexAssist--;
					}
				}
			}
		}
	}
}

/*function for decoding LZ77*/
/*The decoder function receives a file and an array of structures as input.*/
void decoderLZ77(FILE* file, word* arrayWord)
{
	int stop = 0;
	char str[100000] = "";
	int i = 0;
	int w = 0;
	int assistIndexCurrent = 0;
	while (stop != 1)
	{
		/*if the offset is a negative number, then this structure is empty*/
		if (arrayWord[i].offset == -1)
		{
			stop = 1;
			break;
		}
		else
		{
			/*if the structure offset indicates 0, then we take the symbol*/
			if (arrayWord[i].offset == 0)
			{
				str[assistIndexCurrent] = arrayWord[i].s;
				assistIndexCurrent++;
				i++;
			}
			/*if the structure offset contains any number other than 0 and negative,
			then we shift by the specified offset number and repeat current characters*/
			else if (arrayWord[i].offset != -1 && arrayWord[i].offset != 0)
			{
				w = assistIndexCurrent - arrayWord[i].offset;
				for (int z = 0; z < arrayWord[i].count; z++)
				{
					str[assistIndexCurrent] = str[w];
					w++;
					assistIndexCurrent++;
				}
				w = 0;
				i++;
			}
		}
	}
	/*write the decoded value to a file*/
	fputs(str, file);
}
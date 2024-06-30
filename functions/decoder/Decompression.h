//This function decompresses a file compressed with the compression function from the Compression.h library
 
//The function requires the stdlib.h library to work

//The first argument is the file address of the file to be decompressed
//The second argument is the address of the file where the decompressed content should be placed

//The function is designed to work with files up to 150 megabytes in size.If the file size exceeds 
// 150 megabytes, the file must be read in 150 megabyte chunks

//If the work completes successfully, the function returns 1


/*if the character has not been encountered before, it is placed in the structure.
If a symbol has previously been encountered, the distance to its previous occurrence
and the number of repeated symbols are recorded in the structure. the function only
considers the 255 preceding characters. Repeated combinations of less than 4 characters
are not compressed due to impracticality*/
typedef struct wordD
{
	unsigned char offset;
	unsigned char count;
	unsigned char s;
};

/*line where the decompressed content will be placed*/
char stringDec[210000000] = "";

int decompression(char* zip, char* unzip)
{
	FILE* input = fopen(zip, "rb");
	FILE* output = fopen(unzip, "w");
	fseek(input, 0, SEEK_END);
	int countWords = ftell(input);
	fseek(input, 0, SEEK_SET);
	int count = 0;
	/*The array size is immediately set for the maximum possible number of elements in the file.
	This improves decoder performance, but increases memory usage*/
	wordD* arrayWord = (wordD*)malloc(2100000000 * sizeof(wordD));
	int countWrites = 0;
	unsigned char buff;
	for (int i = 0; count < countWords; i++)
	{
		buff = fgetc(input);
		if (buff != '|')
		{
			arrayWord[i].s = buff;
			arrayWord[i].offset = 0;
			arrayWord[i].count = 0;
			countWrites++;
			count++;
		}
		else
		{
			buff = fgetc(input);
			arrayWord[i].offset = buff;
			buff = fgetc(input);
			arrayWord[i].count = buff;
			arrayWord[i].s = '\0';
			countWrites++;
			count += 3;
		}
	}
	int indexStringDec = 0;
	for (int i = 0; i < countWrites; i++)
	{
		if (arrayWord[i].offset == 0)
		{
			stringDec[indexStringDec] = arrayWord[i].s;
			indexStringDec++;
		}
		else
		{
			for (int j = 0; j < arrayWord[i].count; j++)
			{
				stringDec[indexStringDec] = stringDec[indexStringDec - arrayWord[i].offset];
				indexStringDec++;
			}
		}
	}
	fputs(stringDec, output);
	fclose(input);
	fclose(output);
	return 1;
}
#include <math.h>

/*this structure contains information about each unique character*/
typedef struct Table
{
	char word;
	int weight;
	char code[256] = "";
};

/*tree for getting encoding codes*/
typedef struct Tree
{
	char word;
	Tree* left, * right;
};

/*function to convert a number from binary to decimal. Used when writing binary numbers to the output file*/
int convertBinToDec(char* assistStringCode)
{
	int convertedValue = 0;
	int indexAssistChar = 0;
	for (int i = strlen(assistStringCode) - 1; i >= 0; i--)
	{
		convertedValue += ((assistStringCode[indexAssistChar] - 48) * pow(2, i));
		indexAssistChar++;
	}
	return convertedValue;
}

/*function for constructing a Huffman tree and obtaining encoding codes*/
void fillTreeMakeCodes(Tree* tree, int tableLen, int* countWritten, Table* tabST, int* indexTab, char* stringMakeCode, int* indexStringMakeCode)
{
	if (tableLen == *countWritten)
	{
		return;
	}
	if (tableLen - 1 == *countWritten)
	{
		tree->right = (Tree*)malloc(sizeof(Tree));
		tree->left = (Tree*)malloc(sizeof(Tree));
		tree->left->word = tabST[*indexTab].word;
		stringMakeCode[*indexStringMakeCode] = '0';
		strcpy(tabST[*indexTab - 1].code, stringMakeCode);
		*indexTab += 1;
		tree->right->word = tabST[*indexTab].word;
		stringMakeCode[*indexStringMakeCode] = '1';
		strcpy(tabST[*indexTab - 1].code, stringMakeCode);
		*countWritten += 2;
	}
	else
	{
		tree->right = (Tree*)malloc(sizeof(Tree));
		tree->left = (Tree*)malloc(sizeof(Tree));
		tree->left->word = tabST[*indexTab].word;
		stringMakeCode[*indexStringMakeCode] = '0';
		strcpy(tabST[*indexTab - 1].code, stringMakeCode);
		stringMakeCode[*indexStringMakeCode] = '1';
		*indexStringMakeCode += 1;
		*indexTab += 1;
		*countWritten += 1;
		fillTreeMakeCodes(tree->right, tableLen, countWritten, tabST, indexTab, stringMakeCode, indexStringMakeCode);
	}
}

void coderHuffmanStatic(word* arrayWord)
{
	/*We create a table of 256 characters. It will be used to remember the unique symbols encountered and their number*/
	char tableASCII[256] = "";
	for (int i = 0; i < 256; i++)
	{
		tableASCII[i] = 0;
	}
	int numberStruct = 0;
	int indexStruct = 0;
	while (arrayWord[indexStruct].offset != -1)
	{
		numberStruct++;
		indexStruct++;
	}
	int maxStruct = numberStruct;
	numberStruct = 0;
	indexStruct = 0;
	for (int i = 0; i < maxStruct; i++)
	{
		if (arrayWord[i].offset == 0)
		{
			tableASCII[arrayWord[i].s] = arrayWord[i].s;
		}
		else if (arrayWord[i].offset > 0)
		{
			tableASCII[arrayWord[i].offset] = arrayWord[i].offset + 48;
			tableASCII[arrayWord[i].count] = arrayWord[i].count + 48;
		}
	}
	int tableLen = 0;
	for (int i = 0; i < 256; i++)
	{
		if (tableASCII[i] != 0)
		{
			tableLen++;
		}
	}
	Table* tabST = (Table*)malloc(tableLen * sizeof(Table));
	int indexTable = 0;
	for (int i = 0; i < 256; i++)
	{
		if (tableASCII[i] != 0)
		{
			tabST[indexTable].word = tableASCII[i];
			tabST[indexTable].weight = 0;
			indexTable++;
		}
	}
	for (int i = 0; i < maxStruct; i++)
	{
		for (int j = 0; j < tableLen; j++)
		{
			if (arrayWord[i].s == tabST[j].word)
			{
				tabST[j].weight += 1;
			}
			if (arrayWord[i].offset == tabST[j].word - 48)
			{
				tabST[j].weight += 1;
			}
			if (arrayWord[i].count == tabST[j].word - 48)
			{
				tabST[j].weight += 1;
			}
		}
	}
	for (int i = 0; i < tableLen - 1; i++)
	{
		if (tabST[i + 1].weight > tabST[i].weight)
		{
			int weightAssist = tabST[i].weight;
			char wordAssist = tabST[i].word;
			tabST[i].weight = tabST[i + 1].weight;
			tabST[i].word = tabST[i + 1].word;
			tabST[i + 1].weight = weightAssist;
			tabST[i + 1].word = wordAssist;
			i = -1;
		}
	}
	/*create a Huffman tree and get encoding codes*/
	int countWritten = 1;
	int indexTab = 0;
	Tree* tree = (Tree*)malloc(sizeof(Tree));
	tree->left = (Tree*)malloc(sizeof(Tree));
	tree->right = (Tree*)malloc(sizeof(Tree));
	tree->left->word = tabST[indexTab].word;
	indexTab++;
	char stringMakeCode[256] = "";
	int indexStringMakeCode = 0;
	fillTreeMakeCodes(tree->right, tableLen, &countWritten, tabST, &indexTab, stringMakeCode, &indexStringMakeCode);
	char stringCod[100000] = "";
	int indexStringCod = 0;
	while (arrayWord[numberStruct].offset != -1)
	{
		if (arrayWord[numberStruct].offset == 0)
		{
			while (arrayWord[numberStruct].s != tabST[indexStringCod].word)
			{
				indexStringCod++;
			}
			strcat(stringCod, tabST[indexStringCod].code);
			indexStringCod = 0;
			numberStruct++;
		}
		else if (arrayWord[numberStruct].offset > 0)
		{
			while (arrayWord[numberStruct].offset + 48 != tabST[indexStringCod].word)
			{
				indexStringCod++;
			}
			strcat(stringCod, tabST[indexStringCod].code);
			indexStringCod = 0;
			while (arrayWord[numberStruct].count + 48 != tabST[indexStringCod].word)
			{
				indexStringCod++;
			}
			strcat(stringCod, tabST[indexStringCod].code);
			indexStringCod = 0;
			numberStruct++;
		}
	}
	/*We write the encoded content to a file. At the moment, the file is created inside the encoder; 
	later this stage can be moved to another location*/
	FILE* result = fopen("C:\\Users\\andre\\         \\Visual Studio Projects\\LZ77\\Result.txt", "w");
	/*write down the encoding table (number of characters; character; character code; encoded content content)*/
	fputc(tableLen, result);
	char tableCodString[1000] = "";
	int assistVariable;
	for (int i = 0; i < tableLen; i++)
	{
		fputc(tabST[i].word, result);
		fputc(59, result);
		int indexTableCodString = 0;
		for (int j = 0; j < strlen(tabST[i].code); j++)
		{
			tableCodString[indexTableCodString] = tabST[i].code[j];
			indexTableCodString++;
			if (strlen(tableCodString) == 8)
			{
				assistVariable = convertBinToDec(tableCodString);
				fputc(assistVariable, result);
			}
		}
		assistVariable = convertBinToDec(tableCodString);
		fputc(59, result);
	}
	/*write the encoded contents byte by byte*/
	char assistStringCode[9] = "";
	int indexAssistChar = 0;
	while (indexAssistChar < strlen(stringCod))
	{
		for (int i = 0; i < 8; i++)
		{
			assistStringCode[i] = stringCod[indexAssistChar];
			indexAssistChar++;
		}
		assistVariable = convertBinToDec(assistStringCode);
		memset(assistStringCode, '\0', 9);
		fputc(assistVariable, result);
	}
}
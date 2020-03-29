#include <locale>
#include <codecvt>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <io.h>
#include <fcntl.h>
#include "Dictionary.h"
#include "WordsFinder.h"

using namespace std;

int main(int argc, char* argv[])
{
	wchar_t dicttionaryLine[50];
	const char* dictionaryFilePath = "C:\\Users\\durix\\source\\python\\tesserocr\\dictionary\\zdf-utf8.txt";
	wifstream dictionaryFile(dictionaryFilePath);
	vector<wstring> dictionaryData;

	dictionaryFile.imbue(std::locale("en_US.UTF-8"));
	while (dictionaryFile.good())
	{
		dictionaryFile.getline(dicttionaryLine, sizeof(dicttionaryLine));
		dictionaryData.push_back(dicttionaryLine);
	}

	Dictionary dicttionary{ L'а' };

	dicttionary.FillDictionary(dictionaryData);

	const wchar_t charactersBoard[CHARACTERS_BOARD_SIZE][CHARACTERS_BOARD_SIZE] = {
		{ L'к', L'а', L'в', L'ь', L'о' },
		{ L'б', L'а', L'п', L'п', L'с' },
		{ L'а', L'н', L'л', L'е', L'т' },
		{ L'ж', L'и', L'а', L'е', L'р' },
		{ L'с', L'н', L'е', L'м', L'е' },
	};

	WordsFinder finder;

	finder.FindWords((wchar_t*)charactersBoard, &dicttionary);

	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(L"text: %ls\n", L"на русском");
	wcout << L"Found " << finder.GetFoundWords().size() << L" words на русском\n";
	for (auto word : finder.GetFoundWords())
	{
		wcout << word << "\n";
	}

	return 0;
}

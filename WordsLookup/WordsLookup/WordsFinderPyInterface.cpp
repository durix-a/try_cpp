#include "WordsFinderPyInterface.h"
#include <locale>
#include <codecvt>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <io.h>
#include <fcntl.h>

using namespace std;

namespace PyInterface
{
;

WordsFinder::WordsFinder(const char* filePath, wchar_t alphabetFirstCharacter) : _dictionary(alphabetFirstCharacter)
{
	createDictionaryFromFile(filePath);
}

vector<vector<WordCharacter>> WordsFinder::FindWords(const wchar_t* charactersBoard)
{
	_wordsFinder.FindWords(charactersBoard, &_dictionary);

	vector<vector<WordCharacter>> foundWords;
	for (auto word : _wordsFinder.GetFoundWords())
	{
		foundWords.emplace_back();
		vector<WordCharacter>& singleWord = foundWords.back();

		for (auto character : word)
		{
			singleWord.emplace_back(character.character, character.boardX, character.boardY);
		}
	}

	return foundWords;
}

void WordsFinder::createDictionaryFromFile(const char* filePath)
{
	wchar_t dictionaryLine[50];
	wifstream dictionaryFile(filePath);
	vector<wstring> dictionaryData;

	dictionaryFile.imbue(std::locale("en_US.UTF-8"));
	while (dictionaryFile.good())
	{
		dictionaryFile.getline(dictionaryLine, sizeof(dictionaryLine));
		dictionaryData.push_back(dictionaryLine);
	}

	_dictionary.FillDictionary(dictionaryData);
}

wostream& operator<<(wostream& out, const std::vector<WordCharacter>& word)
{
	for (auto character : word)
	{
		out << character.character;
	}
	out << " - ";
	for (auto character : word)
	{
		out << character.character << ":(" << character.boardX << ", " << character.boardY << ") ";
	}

	return out;
}

}

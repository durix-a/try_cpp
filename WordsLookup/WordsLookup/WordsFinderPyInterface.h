#pragma once
#include "WordsFinder.h"
#include <vector>

namespace PyInterface
{
;

struct WordCharacter
{
	WordCharacter() = default;
	WordCharacter(wchar_t ch, int x, int y) : character(ch), boardX(x), boardY(y) {}

	wchar_t character;
	int boardX;
	int boardY;
};

class WordsFinder
{
public:
	WordsFinder(const char* filePath, wchar_t alphabetFirstCharacter);
	std::vector<std::vector<WordCharacter>> FindWords(const wchar_t* charactersBoard);

private:
	void createDictionaryFromFile(const char* filePath);

	WordsLookup::Dictionary _dictionary;
	WordsLookup::WordsFinder _wordsFinder;
};

std::wostream& operator<<(std::wostream& out, const std::vector<WordCharacter>& word);

}

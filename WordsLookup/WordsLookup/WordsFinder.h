#pragma once
#include "Dictionary.h"
#include <vector>
#include <iostream>

#define CHARACTERS_BOARD_SIZE 5

namespace WordsLookup
{
;

struct WordCharacter
{
	WordCharacter() = default;
	WordCharacter(wchar_t ch, int x, int y, Dictionary::DictionaryContext dictionaryLevel) : character(ch), boardX(x), boardY(y), characterDictionaryLevel(dictionaryLevel) {}

	wchar_t character;
	int boardX;
	int boardY;
	Dictionary::DictionaryContext characterDictionaryLevel;
};

class WordsFinder
{
public:
	void FindWords(const wchar_t* charactersBoard, const Dictionary* dictionary);
	const std::vector<std::vector<WordCharacter>>& GetFoundWords() { return _foundWords; }

private:
	const wchar_t* _charactersBoard;
	const Dictionary* _dictionary;
	std::vector<WordCharacter> _currentWord;
	std::vector<std::vector<WordCharacter>> _foundWords;

	void SearchWordsAt(int x, int y);
	bool IsCheckingCurrentWordCharacter(int x, int y);
};

std::wostream& operator<<(std::wostream& out, const std::vector<WordCharacter>& word);

}

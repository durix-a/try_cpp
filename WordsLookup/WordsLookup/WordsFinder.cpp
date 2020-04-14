#include "WordsFinder.h"
#include <queue>

using namespace std;

namespace WordsLookup
{
;

void WordsFinder::FindWords(const wchar_t* charactersBoard, const Dictionary* dictionary)
{
	_charactersBoard = charactersBoard;
	_dictionary = dictionary;

	for (int x = 0; x < CHARACTERS_BOARD_SIZE; x++)
	{
		for (int y = 0; y < CHARACTERS_BOARD_SIZE; y++)
		{
			queue<wchar_t> charactersForPotentialsWords;
			SearchWordsAt(x, y);
		}
	}
}

void WordsFinder::SearchWordsAt(int x, int y)
{
	if (x < 0 || y < 0 || x >= CHARACTERS_BOARD_SIZE || y >= CHARACTERS_BOARD_SIZE || IsCheckingCurrentWordCharacter(x, y))
	{
		return;
	}

	wchar_t character = _charactersBoard[x + y * CHARACTERS_BOARD_SIZE];
	Dictionary::DictionaryContext dictionaryLevel;

	if (_currentWord.empty())
	{
		dictionaryLevel = _dictionary->IsWordFirstCharacterExist(character);
	}
	else
	{
		dictionaryLevel = _dictionary->IsWordCharacterExist(_currentWord.back().characterDictionaryLevel, character);
	}
	if (dictionaryLevel == nullptr)
	{
		return;
	}

	_currentWord.emplace_back(character, x, y, dictionaryLevel);

	if (dictionaryLevel->isWordEnd)
	{
		_foundWords.push_back(_currentWord);
	}

	SearchWordsAt(x - 1, y - 1);
	SearchWordsAt(x - 0, y - 1);
	SearchWordsAt(x + 1, y - 1);
	SearchWordsAt(x - 1, y - 0);
	SearchWordsAt(x + 1, y - 0);
	SearchWordsAt(x - 1, y + 1);
	SearchWordsAt(x - 0, y + 1);
	SearchWordsAt(x + 1, y + 1);
	_currentWord.pop_back();
}

bool WordsFinder::IsCheckingCurrentWordCharacter(int x, int y)
{
	for (auto character : _currentWord)
	{
		if (x == character.boardX && y == character.boardY)
		{
			return true;
		}
	}
	return false;
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

#include "Dictionary.h"

namespace WordsLookup
{
;

const Dictionary::DictionaryContext Dictionary::IsWordFirstCharacterExist(wchar_t character) const
{
	return IsWordCharacterExist(&_baseDictionaryLevel, character);
}

const Dictionary::DictionaryContext Dictionary::IsWordCharacterExist(const DictionaryContext currentDictionaryLevel, wchar_t character) const
{
	int characterIndex = character - _alphabetFirstCharacter;
	const auto currentCharacterIter = currentDictionaryLevel->dictionaryLevelCharacters.find(characterIndex);
	if (currentCharacterIter == currentDictionaryLevel->dictionaryLevelCharacters.end())
	{
		return nullptr;
	}

	return currentCharacterIter->second.get();
}

std::pair<bool, bool> Dictionary::IsWordPartExist(const std::wstring& wordPart) const
{
	const DictionaryLevel* currentDictionaryLevel = &_baseDictionaryLevel;

	for (auto character : wordPart)
	{
		currentDictionaryLevel = IsWordCharacterExist(currentDictionaryLevel, character);
		if (currentDictionaryLevel == nullptr)
		{
			return std::make_pair(false, false);
		}
	}

	return std::make_pair(true, currentDictionaryLevel->isWordEnd);
}

void Dictionary::FillDictionary(const std::vector<std::wstring>& allWords)
{
	for (auto word : allWords)
	{
		DictionaryLevel* currentDictionaryLevel = &_baseDictionaryLevel;

		for (auto character : word)
		{
			int characterIndex = character - _alphabetFirstCharacter;
			DictionaryLevelPtr& currentCharacterPtr = currentDictionaryLevel->dictionaryLevelCharacters[characterIndex];
			if (currentCharacterPtr.get() == nullptr)
			{
				currentCharacterPtr.reset(new DictionaryLevel);
			}

			currentDictionaryLevel = currentCharacterPtr.get();
		}

		currentDictionaryLevel->isWordEnd = true;
	}
}

}

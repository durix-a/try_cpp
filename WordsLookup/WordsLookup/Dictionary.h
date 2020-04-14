#pragma once
#include <array>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

namespace WordsLookup
{
;

class Dictionary
{
public:
	struct DictionaryLevel;
	using DictionaryContext = const DictionaryLevel*;
	typedef std::unique_ptr<DictionaryLevel> DictionaryLevelPtr;
	struct DictionaryLevel
	{
		bool isWordEnd = false;
		std::unordered_map<int, DictionaryLevelPtr> dictionaryLevelCharacters;
	};

	Dictionary(wchar_t alphabetFirstCharacter) : _alphabetFirstCharacter(alphabetFirstCharacter) {}
	const DictionaryContext IsWordFirstCharacterExist(wchar_t character) const;
	const DictionaryContext IsWordCharacterExist(const DictionaryContext characterContext, wchar_t character) const;
	std::pair<bool, bool> IsWordPartExist(const std::wstring& word) const;
	void FillDictionary(const std::vector<std::wstring>& allWords);

private:
	wchar_t _alphabetFirstCharacter;
	DictionaryLevel _baseDictionaryLevel;
};

}

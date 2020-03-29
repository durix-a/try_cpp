#include "gtest/gtest.h"
#include "../WordsLookup/Dictionary.h"

using namespace std;

class WordsDictionaryTest : public testing::Test
{
public:
    WordsDictionaryTest()
    {
        dict.FillDictionary(dictionaryWords);
    }

protected:
    vector<wstring> dictionaryWords{
        L"aaa", L"ace", L"bft",
        L"aag", L"aah", L"aaj", L"aak",
        L"aaaa", L"acez", L"zzz", L"zzzz"
    };

    Dictionary dict{ L'a' };
};

TEST_F(WordsDictionaryTest, TestDictionaryCharacters)
{
    Dictionary::DictionaryContext dictLevelSaved;
    Dictionary::DictionaryContext dictLevel = dict.IsWordFirstCharacterExist(L'a');
    EXPECT_NE(nullptr, dictLevel);
    dictLevel = dict.IsWordCharacterExist(dictLevel, L'a');
    EXPECT_NE(nullptr, dictLevel);
    dictLevel = dict.IsWordCharacterExist(dictLevel, L'a');
    EXPECT_NE(nullptr, dictLevel);
    dictLevel = dict.IsWordCharacterExist(dictLevel, L'a');
    EXPECT_NE(nullptr, dictLevel);
    dictLevel = dict.IsWordCharacterExist(dictLevel, L'a');
    EXPECT_EQ(nullptr, dictLevel);

    dictLevel = dict.IsWordFirstCharacterExist(L'd');
    EXPECT_EQ(nullptr, dictLevel);

    dictLevel = dict.IsWordFirstCharacterExist(L'b');
    EXPECT_NE(nullptr, dictLevel);
    dictLevel = dict.IsWordCharacterExist(dictLevel, L'g');
    EXPECT_EQ(nullptr, dictLevel);

    dictLevel = dict.IsWordFirstCharacterExist(L'b');
    EXPECT_NE(nullptr, dictLevel);
    dictLevel = dict.IsWordCharacterExist(dictLevel, L'f');
    EXPECT_NE(nullptr, dictLevel);
    dictLevelSaved = dictLevel;
    dictLevel = dict.IsWordCharacterExist(dictLevel, L'a');
    EXPECT_EQ(nullptr, dictLevel);
    dictLevel = dict.IsWordCharacterExist(dictLevelSaved, L't');
    EXPECT_NE(nullptr, dictLevel);
}

TEST_F(WordsDictionaryTest, TestDictionaryWordPart)
{
    EXPECT_EQ(dict.IsWordPartExist(L"ace"), make_pair(true, true));
    EXPECT_EQ(dict.IsWordPartExist(L"aa"), make_pair(true, false));
    EXPECT_EQ(dict.IsWordPartExist(L"aaa"), make_pair(true, true));
    EXPECT_EQ(dict.IsWordPartExist(L"aaaa"), make_pair(true, true));
    EXPECT_EQ(dict.IsWordPartExist(L"aaaaa"), make_pair(false, false));
    EXPECT_EQ(dict.IsWordPartExist(L"bft"), make_pair(true, true));
    EXPECT_EQ(dict.IsWordPartExist(L"bf"), make_pair(true, false));
    EXPECT_EQ(dict.IsWordPartExist(L"abe"), make_pair(false, false));
    EXPECT_EQ(dict.IsWordPartExist(L"aai"), make_pair(false, false));
    EXPECT_EQ(dict.IsWordPartExist(L"aak"), make_pair(true, true));
    EXPECT_EQ(dict.IsWordPartExist(L"zz"), make_pair(true, false));
    EXPECT_EQ(dict.IsWordPartExist(L"zzz"), make_pair(true, true));
    EXPECT_EQ(dict.IsWordPartExist(L"zzzz"), make_pair(true, true));
    EXPECT_EQ(dict.IsWordPartExist(L"zzzzz"), make_pair(false, false));
}
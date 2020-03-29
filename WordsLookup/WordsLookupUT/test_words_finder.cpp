#include "gtest/gtest.h"
#include "../WordsLookup/WordsFinder.h"
#include <iostream>

using namespace std;

bool operator==(const WordCharacter& compareWord, const WordCharacter& compareWith)
{
    return (compareWord.character == compareWith.character && compareWord.boardX == compareWith.boardX && compareWord.boardY == compareWith.boardY);
}

class WordsFinderTest : public testing::Test
{
protected:
    wchar_t* wordsBoard = 
        L"afafa"
        L"gbgbg"
        L"chchc"
        L"ididi"
        L"ejeje";
    wchar_t* wordsBoardAlphabet =
        L"abcde"
        L"fghij"
        L"klmno"
        L"pqrst"
        L"uvwxy";

    Dictionary dict{ L'a' };
};

TEST_F(WordsFinderTest, TestWordsFinderAlphabet)
{
    vector<wstring> dictionaryWords{
        L"abc", L"fgcbh", L"eihnj", L"qlmkghi"
    };

    dict.FillDictionary(dictionaryWords);

    WordsFinder finder;
    finder.FindWords(wordsBoardAlphabet, &dict);
    EXPECT_EQ(3, finder.GetFoundWords().size());

    vector<WordCharacter> word0{ {L'a', 0, 0, nullptr}, {L'b', 1, 0, nullptr}, {L'c', 2, 0, nullptr} };
    EXPECT_EQ(word0, finder.GetFoundWords()[0]);
    vector<WordCharacter> word1{ {L'f', 0, 1, nullptr}, {L'g', 1, 1, nullptr}, {L'c', 2, 0, nullptr}, {L'b', 1, 0, nullptr}, {L'h', 2, 1, nullptr} };
    EXPECT_EQ(word1, finder.GetFoundWords()[1]);
    vector<WordCharacter> word2{ {L'e', 4, 0, nullptr}, {L'i', 3, 1, nullptr}, {L'h', 2, 1, nullptr}, {L'n', 3, 2, nullptr}, {L'j', 4, 1, nullptr} };
    EXPECT_EQ(word2, finder.GetFoundWords()[2]);

    //for (auto word : finder.GetFoundWords())
    //{
    //    wcout << word << "\n";
    //}
}

TEST_F(WordsFinderTest, TestWordsFinder5Words)
{
    vector<wstring> dictionaryWords{
        L"abc"
    };

    dict.FillDictionary(dictionaryWords);

    WordsFinder finder;
    finder.FindWords(wordsBoard, &dict);
    EXPECT_EQ(8, finder.GetFoundWords().size());
}

TEST_F(WordsFinderTest, TestWordsFinderAbc)
{
    vector<wstring> dictionaryWords{
        L"abc"
    };

    dict.FillDictionary(dictionaryWords);

    WordsFinder finder;
    finder.FindWords(wordsBoard, &dict);
    EXPECT_EQ(8, finder.GetFoundWords().size());
}

TEST_F(WordsFinderTest, TestWordsFinderAbcde)
{
    vector<wstring> dictionaryWords{
        L"abcde"
    };

    dict.FillDictionary(dictionaryWords);

    WordsFinder finder;
    finder.FindWords(wordsBoard, &dict);
    EXPECT_EQ(24, finder.GetFoundWords().size());
}

TEST_F(WordsFinderTest, TestWordsFinderAfabch)
{
    vector<wstring> dictionaryWords{
        L"afabch"
    };

    dict.FillDictionary(dictionaryWords);

    WordsFinder finder;
    finder.FindWords(wordsBoard, &dict);
    EXPECT_EQ(18, finder.GetFoundWords().size());
}

TEST_F(WordsFinderTest, TestWordsFinderFbhdj)
{
    vector<wstring> dictionaryWords{
        L"fbhdj"
    };

    dict.FillDictionary(dictionaryWords);

    WordsFinder finder;
    finder.FindWords(wordsBoard, &dict);
    EXPECT_EQ(2, finder.GetFoundWords().size());
}

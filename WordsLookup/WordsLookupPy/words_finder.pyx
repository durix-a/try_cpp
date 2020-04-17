# distutils: language=c++

from WordsFinder cimport WordCharacter
from WordsFinder cimport WordsFinder
from libcpp.vector cimport vector

cdef class WordsFinderWrapper:
    cdef WordsFinder* _words_finder

    def __init__(self, filePath, alphabetFirstCharacter):
        self._words_finder = new WordsFinder(filePath, alphabetFirstCharacter)

    def __dealloc__(self):
        del self._words_finder

    def find_words(self, charactersBoard):
        cdef vector[vector[WordCharacter]] found_words

        # charactersBoardDecoded = charactersBoard.decode("UTF-8")
        # found_words = self._words_finder.FindWords(charactersBoard.decode("UTF-8"))
        found_words = self._words_finder.FindWords(charactersBoard)

        returned_words = []
        for word in found_words:
            returned_words.append([{ "character": character.character, "x": character.boardX, "y": character.boardY } for character in word])
        
        return returned_words


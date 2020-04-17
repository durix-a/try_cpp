from libcpp.vector cimport vector

cdef extern from "WordsFinderPyInterface.h" namespace "PyInterface":
    cdef cppclass WordCharacter:
        WordCharacter()
        WordCharacter(Py_UNICODE ch, int x, int y)
        Py_UNICODE character
        int boardX
        int boardY

    cdef cppclass WordsFinder:
        WordsFinder(const char* filePath, Py_UNICODE alphabetFirstCharacter)
        vector[vector[WordCharacter]] FindWords(const Py_UNICODE* charactersBoard)


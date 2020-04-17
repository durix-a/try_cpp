import words_finder

wf = words_finder.WordsFinderWrapper(b'C:/Users/durix/source/python/tesserocr/dictionary/zdf-utf8.txt', ord(u'а'))

#         1    2    3    4    5    
board = u'аседдкзчзивфаинтвкатиесит'
# board = u'кавьобаппсанлетжиаерснеме'
# found_words = wf.find_words(board.encode("UTF-8"))
found_words = wf.find_words(board)

def print_word(word):
    word_text = ""
    word_coordinates = ""

    for character in word:
        word_text += character['character']
        word_coordinates += "{0}:({1},{2}) ".format(character['character'], character['x'], character['y'])
    
    print("{0}: - {1})".format(word_text, word_coordinates))

for word in found_words:
    print_word(word)
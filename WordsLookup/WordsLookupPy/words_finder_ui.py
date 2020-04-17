import tkinter
import tkinter.font as tkfont
import math
from functools import partial
import words_finder

def get_word_text(word):
    word_text = ""

    for character in word:
        word_text += character['character']
    
    return word_text

def get_word_sort_key(word):
    return "{0:0>3}{1}".format(len(word), get_word_text(word))

def get_grid_square_size(characters_set):
    characters_set_size = math.sqrt(len(characters_set))
    if math.floor(characters_set_size) == characters_set_size:
        characters_set_size = characters_set_size
    else:
        characters_set_size = math.floor(characters_set_size) + 1
    
    return int(characters_set_size)

class CharacterSelector(tkinter.Frame):
    def __init__(self, master, character_selected_handler):
        super().__init__(master)
        self.font = tkfont.Font(family='courier', size=25)
        self.character_selected_handler = character_selected_handler

    def create_character_buttons(self, characters_set):
        characters_set_size = get_grid_square_size(characters_set)

        for x in range(characters_set_size):
            for y in range(characters_set_size):
                characters_set_index = y * characters_set_size + x
                
                if characters_set_index >= len(characters_set):
                    break
                
                button_options = {
                    "text": characters_set[characters_set_index], 
                    "font": self.font
                }
                
                button = tkinter.Button(self, button_options)
                button.grid(column=x, row=y)
                button['command'] = partial(self.character_selected_handler, self, button)


class CharactersBoard(tkinter.Frame):
    def __init__(self, master, alphabet):
        super().__init__(master)
        self.alphabet = alphabet
        self.font = tkfont.Font(family='courier', size=25)
        self.last_pressed_button = None
        self.board_widgets = None

    def create_character_board(self, characters_set):
        self.characters_set_size = get_grid_square_size(characters_set)
        self.characters_set = list(characters_set)
        self.board_widgets = []
        is_first_row = True

        for y in range(self.characters_set_size):
            if not is_first_row:
                self.__add_labels_only_row(y)
            else:
                is_first_row = False

            self.__add_buttons_row(y)

        self.characters_set_editor = tkinter.Text(self, width=5, height=5, font='courier 15')
        self.characters_set_editor.grid(column=0, row=(self.characters_set_size * 2), columnspan=2, rowspan=3)
        self.characters_set_editor.insert(tkinter.END, characters_set)
        tkinter.Button(self, text='update characters', command=self.__update_characters).grid(column=2, row=(self.characters_set_size * 2), columnspan=3)
    
    def set_word(self, word):
        previous_character = None

        for character in word:
            button_y = character['y'] * 2
            button_x = character['x'] * 2
            button = self.board_widgets[button_y][button_x]
            assert(character['character'] == button['text']), "word's character '{0}' doesn't match button character '{1}'. (x={2}, y={3})".format(character['character'], button['text'], character['x'], character['y'])
            button['bg'] = 'yellow'

            if previous_character:
                button['bg'] = 'yellow'
                self.__set_label_arrow(previous_character, character)
            else:
                button['bg'] = 'blue'

            previous_character = character

    def clear_word(self):
        for widget_row in self.board_widgets:
            for widget in widget_row:
                if isinstance(widget, tkinter.Label):
                    widget['text'] = ' '
                if isinstance(widget, tkinter.Button):
                    widget['bg'] = "SystemButtonFace"

    def get_characters_set(self):
        return "".join(self.characters_set)

    def __add_buttons_row(self, row_index):
        is_first_button = True
        board_widgets_row = []

        for x in range(self.characters_set_size):
            characters_set_index = row_index * self.characters_set_size + x
            
            if characters_set_index >= len(self.characters_set):
                break

            if not is_first_button:
                label = self.__create_label()
                label.grid(column=(x*2-1), row=(row_index * 2))
                board_widgets_row.append(label)
            
            is_first_button = False
            button = self.__create_button()
            button.grid(column=(x*2), row=(row_index * 2))
            button['text'] = self.characters_set[characters_set_index]
            button.characters_set_index = characters_set_index
            board_widgets_row.append(button)

        self.board_widgets.append(board_widgets_row)

    def __add_labels_only_row(self, row_index):
        board_widgets_row = []

        for x in range(self.characters_set_size * 2 - 1):
            label = self.__create_label()
            label.grid(column=x, row=(row_index * 2 - 1))
            board_widgets_row.append(label)

        self.board_widgets.append(board_widgets_row)
            
    def __create_button(self):
        button = tkinter.Button(self, font=self.font)
        button['command'] = partial(self.__change_character, self, button)

        return button

    def __create_label(self):
        return tkinter.Label(self, font=self.font, text=' ')

    def __get_arrow_character(self, delta_x, delta_y):
        return [
            chr(0x2196), 
            chr(0x2191), 
            chr(0x2197), 
            chr(0x2190), 
            '*',
            chr(0x2192), 
            chr(0x2199), 
            chr(0x2193), 
            chr(0x2198)
        ][(delta_y + 1) * 3 + (delta_x + 1)]

    def __set_label_arrow(self, previous_character, current_character):
        delta_x = current_character['x'] - previous_character['x']
        delta_y = current_character['y'] - previous_character['y']
        assert(delta_x >= -1 and delta_x <= 1 and delta_y >= -1 and delta_y <= 1), "current character must be adjacent to previous character (delta = 0/1)"
        label_x = current_character['x'] * 2 - delta_x
        label_y = current_character['y'] * 2 - delta_y
        label = self.board_widgets[label_y][label_x]

        if label['text'] == ' ':
            label['text'] = self.__get_arrow_character(delta_x, delta_y)
        else:
            label['text'] += self.__get_arrow_character(delta_x, delta_y)

    def __character_selected_handler(self, parent_window, pressed_button):
        self.last_pressed_button['text'] = pressed_button['text']
        self.characters_set[self.last_pressed_button.characters_set_index] = pressed_button['text']
        parent_window.winfo_toplevel().destroy()
        self.characters_set_editor.delete(1.0, tkinter.END)
        self.characters_set_editor.insert(tkinter.END, "".join(self.characters_set))

    def __change_character(self, parent_window, pressed_button):
        self.last_pressed_button = pressed_button
        window = tkinter.Toplevel()
        alphabet_ui = CharacterSelector(window, self.__character_selected_handler)
        alphabet_ui.pack()
        alphabet_ui.create_character_buttons(alphabet)

    def __update_characters(self):
        self.clear_word()
        updated_characters = self.characters_set_editor.get(1.0, float(len(self.characters_set)))
        assert(len(updated_characters) >= len(self.characters_set)), "not enough characters to fill character set"
        self.characters_set[:] = list(updated_characters)
        characters_set_iter = iter(self.characters_set)

        for y in range(0, self.characters_set_size * 2, 2):
            for x in range(0, self.characters_set_size * 2, 2):
                self.board_widgets[y][x]['text'] = next(characters_set_iter)
            

class WordsSelector(tkinter.Frame):
    def __init__(self, master, characters_board):
        super().__init__(master)
        self.characters_board = characters_board
        self.font = tkfont.Font(family='courier', size=15)
        self.scrollbar = tkinter.Scrollbar(self, orient=tkinter.VERTICAL)
        self.listbox = tkinter.Listbox(self, selectmode=tkinter.SINGLE, yscrollcommand=self.scrollbar.set, font=self.font)
        self.scrollbar.config(command=self.listbox.yview)
        self.scrollbar.pack(side=tkinter.RIGHT, fill=tkinter.Y)
        self.listbox.pack(side=tkinter.LEFT, fill=tkinter.BOTH, expand=1)
        self.listbox.bind('<<ListboxSelect>>', self.__onselect)
        self.words_list = None
        self.title_label = tkinter.Label(self, font=self.font, text="words: 0")
        self.title_label.pack(side=tkinter.TOP)

    def set_words_list(self, words_list):
        self.words_list = words_list
        self.listbox.delete(0, tkinter.END)

        for word in words_list:
            word_text = ""
            
            for character in word:
                word_text += character['character']
            
            self.listbox.insert(tkinter.END, word_text)
        
        self.title_label['text'] = "words: {0}".format(self.listbox.size())

    def __onselect(self, event):
        word_index = self.listbox.curselection()[0]
        self.characters_board.clear_word()
        self.characters_board.set_word(self.words_list[word_index])


class ActionsUI(tkinter.Frame):
    def __init__(self, master, characters_board, words_selector):
        super().__init__(master)
        self.characters_board = characters_board
        self.words_selector = words_selector
        self.words_finder = words_finder.WordsFinderWrapper(b'C:/Users/durix/source/python/tesserocr/dictionary/zdf-utf8.txt', ord(u'а'))
        tkinter.Button(self, font="courier 15", text="find words", command=self.__on_find_words_pressed).pack()
        tkinter.Button(self, font="courier 15", text="clear board", command=self.__on_clear_board_pressed).pack()

    def __on_find_words_pressed(self):
        board_characters = self.characters_board.get_characters_set()
        found_words = self.words_finder.find_words(board_characters)
        found_words[:] = filter(lambda word: len(word) > 1, found_words)
        found_words.sort(key=get_word_sort_key, reverse=True)
        self.words_selector.set_words_list(found_words)

    def __on_clear_board_pressed(self):
        self.characters_board.clear_word()


alphabet = u'абвгдежзийклмнопрстуфхцчшщъыьэюя'
board = u'аседдкзчзивфаинтвкатиесит'

ui_root = tkinter.Tk()
board_ui = CharactersBoard(ui_root, alphabet)
board_ui.pack(side=tkinter.LEFT)
board_ui.create_character_board(board)

words_list = [
                [{'character':u'а', 'x':0, 'y':0}, 
                    {'character':u'з', 'x':1, 'y':1}, 
                    {'character':u'а', 'x':2, 'y':2}], 
                [{'character':u'а', 'x':0, 'y':0}, 
                    {'character':u'с', 'x':1, 'y':0}, 
                    {'character':u'е', 'x':2, 'y':0}, 
                    {'character':u'ч', 'x':2, 'y':1}, 
                    {'character':u'з', 'x':3, 'y':1}, 
                    {'character':u'и', 'x':4, 'y':1}, 
                    {'character':u'н', 'x':4, 'y':2}, 
                    {'character':u'и', 'x':3, 'y':2}, 
                    {'character':u'а', 'x':2, 'y':2}, 
                    {'character':u'к', 'x':2, 'y':3}],
                [{'character':u'а', 'x':0, 'y':0}, 
                    {'character':u'з', 'x':1, 'y':1}, 
                    {'character':u'е', 'x':2, 'y':0}, 
                    {'character':u'ч', 'x':2, 'y':1}, 
                    {'character':u'ф', 'x':1, 'y':2}, 
                    {'character':u'т', 'x':0, 'y':3}, 
                    {'character':u'в', 'x':1, 'y':3}, 
                    {'character':u'в', 'x':0, 'y':2}, 
                    {'character':u'к', 'x':0, 'y':1}, 
                    {'character':u'с', 'x':1, 'y':0}]
]

# board_ui.set_word(words_list[2])
words_list_ui = WordsSelector(ui_root, board_ui)
words_list_ui.pack(side=tkinter.RIGHT, fill=tkinter.BOTH, expand=True)
# words_list_ui.set_words_list(words_list)

actions_ui = ActionsUI(ui_root, board_ui, words_list_ui)
actions_ui.pack(side=tkinter.BOTTOM)

ui_root.mainloop()
print("the END")
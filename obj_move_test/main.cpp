#include <iostream>
#include <algorithm>
#include <random>

using namespace std;

class MoveTest {
    char* _ptr = nullptr;
    int _num = 0;

public:
    const char* getPtr() const { return _ptr; }
    int getNum() const { return _num; }

    MoveTest(MoveTest&& obj) {
        cout << __FUNCTION__ << "(move) called. " << this << " " << obj._ptr << " " << obj._num << endl;
        _ptr = obj._ptr;
        _num = obj._num;
    }

    MoveTest(char* ptr, int num) : _num(num) {
        cout << __FUNCTION__ << "(init) called. " << this << " " << ptr << " " << num << endl;
        _ptr = new char[num];
        copy_n(ptr, num, _ptr);
    }

    ~MoveTest() {
        cout << __FUNCTION__ << " called. " << this << " " << (_ptr == nullptr ? "moved" : _ptr) << " " << _num << endl;
        if(_ptr != nullptr) {
            delete _ptr;
            _ptr = nullptr;
            _num = 0;
        }
    }

    MoveTest& concat(const MoveTest& obj) {
        cout << __FUNCTION__ << " called. " << _ptr << " " << _num << " + " << obj._ptr << " " << obj._num << endl;

        char* temp_ptr  = new char[_num + obj._num];
        
        copy_n(_ptr, _num, temp_ptr);
        temp_ptr[_num - 1] = ' ';
        copy_n(obj._ptr, obj._num, temp_ptr + _num);

        _num += obj._num;
        delete _ptr;
        _ptr = temp_ptr;
    }
};

void PrintMoveTest(MoveTest mt) {
    if(mt.getPtr() == nullptr) {
        cout << __FUNCTION__ << ": " << "empty" << endl;
    } else {
        cout << __FUNCTION__ << ": " << mt.getPtr() << " " << mt.getNum() << endl;
    }
}

MoveTest CreateRandomMoveTest() {
    std::random_device r;
 
    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist(10, 20);
    int num = uniform_dist(e1);
    vector<char> str;
    std::uniform_int_distribution<char> uniform_dist1('a', 'z');

    str.resize(num);
    for(int i = 0; i < num - 1; i++) {
        str[i] = uniform_dist1(e1);
    }

    str[num - 1] = '\0';

    return MoveTest(&str[0], num);
}

int main(int argc, char* argv[]) {
    cout << "started" << endl;
    char str[] = "hello world";
    char str1[] = "temporary object";
    char str2[] = "temporary object2";
    char str12[] = "temporary object12";
    char str123[] = "temporary object123";
    
    // MoveTest mt = {str, sizeof(str)};
    // cout << "direct: " << mt.getPtr() << " " << mt.getNum() << endl;
    // PrintMoveTest(mt);
    
    PrintMoveTest(MoveTest(str1, sizeof(str1)));
    
    // MoveTest mt1 = CreateRandomMoveTest();
    // PrintMoveTest(mt1);

    // MoveTest mt2 = { str2, sizeof(str2) };
    // PrintMoveTest(mt2);
    
    PrintMoveTest({ str12, sizeof(str12) });

    // MoveTest& mt3 = CreateRandomMoveTest().concat(mt).concat({ str123, sizeof(str123) });
    // PrintMoveTest(mt3);

    // MoveTest mt4 = move(CreateRandomMoveTest().concat(mt1).concat(mt2));
    // PrintMoveTest(mt4);

    cout << "finished" << endl;
}
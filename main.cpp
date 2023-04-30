#include "smart_ptr.hpp"

// A very small amount of tests

#define MAKE_TEST(name, code) bool RUN_TEST_ ## name ## __ () { {code} return true; }
#define ASSERT(cond) if (!(cond)) { return false; }
#define RUN_TEST(name)\
    if (RUN_TEST_ ## name ## __ ()) {\
        printf("\033[;32mTest %s passed!\033[0m\n", #name);\
    } else {\
        printf("\033[;31mTest %s FAILED!\033[0m\n", #name);\
    }


MAKE_TEST(trivial, 
    ASSERT(1 + 1 == 2);
)

// 1 warning
MAKE_TEST(simple,
    smart_ptr<int> data = new int[10];
    data = data;
    data[0] = 3;
    ASSERT(data[0] == 3);
)

// no warnings
MAKE_TEST(connectedness,
    auto v = [](){
        smart_ptr<int> data = new int[1];
        if (!data) {
            // ... erm
        }
        data[0] = 1;
        return data;
    };
    smart_ptr<int> data;
    data = v();
    ASSERT(data[0] == 1);
)

// has 1 warning
MAKE_TEST(connectedness2,
    auto u = [](){
        smart_ptr<int> data = new int[1];
        smart_ptr<int> res = data; // here we copy and forget to check that res is good
        if (!data) {
            // ... erm
        }
        data[0] = 1;
        data = data;
        return res;
    };
    smart_ptr<int> data = u();
    ASSERT(data[0] == 1);
)

// no warnings
MAKE_TEST(lifetimes,
    auto v = [](int size){ 
        smart_ptr<int> data = new int[size];
        if (!data) {
            throw std::logic_error("impossible");
        }

        smart_ptr<int> res = data;
        for (int i = 0; i < size; i++) {
            data[i] = i;
        }
        return res;
    };
    smart_ptr<int> val;
    val = v(3);
    ASSERT(val[0] == 0 && val[1] == 1);
)

// no warnings
MAKE_TEST(move, 
    smart_ptr<int> fst = new int[1];
    if (!fst) {
        throw std::logic_error("impossible");
    }
    fst[0] = 1;
    fst = std::move(fst);
    ASSERT(fst && fst[0] == 1);
    smart_ptr<int> snd;
    snd = std::move(fst);
    ASSERT(!fst)
    ASSERT(snd[0] == 1)
)



int main() {
    RUN_TEST(trivial);
    RUN_TEST(simple);
    RUN_TEST(connectedness);
    RUN_TEST(connectedness2);
    RUN_TEST(lifetimes);
    RUN_TEST(move);
}
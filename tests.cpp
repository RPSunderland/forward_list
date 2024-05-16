#include "forward_list.hpp"
#include <forward_list>
#include <assert.h>

void test1() {
    rafal::forward_list<int> l1;
    l1.push_front(5);
    l1.push_front(10);
    l1.push_front(15);
    assert(l1.front() == 15);    
}

void test2() {
    rafal::forward_list<int> l2;
    l2.push_front(5);
    l2.push_front(10);
    l2.push_front(15);
    assert(l2.empty() == false);
}

void test3() {
    rafal::forward_list<int> l3;
    assert(l3.empty() == true);
}

void test4() {
    rafal::forward_list<int> l4;
    l4.push_front(5);
    l4.push_front(10);
    l4.push_front(15);
    l4.pop_front();
    assert(l4.front() == 10);
}

void test5() {
    rafal::forward_list<int> l5;
    l5.push_front(5);
    l5.push_front(10);
    l5.push_front(15);
    l5.remove(10);
    assert(l5.front() == 15);
}

void test6() {
    std::forward_list<int> l6;
    l6.push_front(5);
    l6.push_front(10);
    l6.push_front(15);
    auto it = l6.begin();
    ++it;
    ++it;
    l6.insert_after(it, 20);
    assert(l6.front() == 15 && *++l6.begin() == 10 && *++(++l6.begin()) == 5 && *++(++(++l6.begin())) == 20);
}

void test7() {
    rafal::forward_list<int> l7;
    assert(l7.max_size() > 0);
}

void test8() {
    rafal::forward_list<int> l8;
    assert(l8.empty() == true);
}

void test9() {
    rafal::forward_list<int> l9;
    l9.push_front(5);
    l9.push_front(10);
    l9.push_front(15);
    l9.push_front(20);
    assert(l9.size() == 4);
}

void test10() {
    rafal::forward_list<int> l10;
    l10.push_front(5);
    l10.push_front(10);
    l10.push_front(15);
    l10.push_front(20);
    l10.clear();
    assert(l10.empty() == true);
}

void test11() {
    rafal::forward_list<int> l11;
    assert(l11.before_begin().node_ptr != nullptr);
}

void test12() {
    rafal::forward_list<int> l13;
    for (int i = 0; i < 1000; ++i) {
        l13.push_front(i);
    }
    assert(l13.size() == 1000);
}

void test13() {
    rafal::forward_list<int> l14;
    for (int i = 0; i < 1000; ++i) {
        l14.push_front(i);
    }
    l14.resize(500);
    assert(l14.size() == 500);
}

void test14() {
    rafal::forward_list<int> l15;
    for (int i = 0; i < 1000; ++i) {
        l15.push_front(i);
    }
    l15.resize(1500);
    assert(l15.size() == 1500);
}

void test15() {
    rafal::forward_list<int> l16;
    
    for (int i = 0; i < 1000; ++i) {
        l16.push_front(i);
    }
    l16.resize(2000, 10);
    auto it = l16.begin();
    for (int i = 0; i <= 1500; ++i) {
        ++it;
    }
    assert(l16.size() == 2000 && *it == 10);
}

void test16() {
    rafal::forward_list<int> l17;
    for (int i = 0; i < 1000; ++i) {
        l17.push_front(i);
    }
    rafal::forward_list<int> l18;
    for (int i = 0; i < 500; ++i) {
        l18.push_front(i);
    }
    l17.swap(l18);
    assert(l17.size() == 500 && l18.size() == 1000);
}

void test17() {
    rafal::forward_list<int> l19;
    rafal::forward_list<int> l20;
    l19.swap(l20);
    assert(l19.empty() == true && l20.empty() == true);
}

void test18() {
    rafal::forward_list<int> l21;
    for (int i = 0; i < 1000; ++i) {
        l21.push_front(i);
    }
    auto l22 = l21.split_when([](const int& value) { return value == 500; });
    assert(l21.size() == 499 && l22.size() == 501);
}

void test19() {
    rafal::forward_list<int> l23;
    for (int i = 0; i < 1000; ++i) {
        l23.push_front(i);
    }
    auto l24 = l23.split_when([](const int& value) { return value == 2000; });
    assert(l23.size() == 1000 && l24.size() == 0);
}

int main() {
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
    test8();
    test9();
    test10();
    test11();
    test12();
    test13();
    test14();
    test15();
    test16();
    test17();
    test18();
    test19();
    return 0;
}

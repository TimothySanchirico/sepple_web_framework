#include <iostream>
#include <functional>
#include "prefix_tree.h"


std::string f(int n){
        return std::string("Hello Poop!\n");
}

std::string f2(int n){
        std::string s("Fuck this couch\n");
        return s;
}

int c;
bool z1(){c++; return true;}
bool z2(){std::cout << c << std::endl; return false;}
int main(void){
        c = 4;
        PrefixTree<std::function<std::string(int n)> > p;
        std::string key("apples");
        std::string key2("apples2");
        p.insert(key, f);
        p.insert(key2, f2);
        std::cout << p.get(key)(4);
        std::cout << p.get(key2)(5);
        std::cout << p.get(key)(4);
        int x;
        std::cout << (x = 6) << std::endl;;
        if(z1() && z2()){
                std::cout << "FSHACK" << std::endl;
        }
        else{ std::cout << c << std::endl; }
        return 0;
}

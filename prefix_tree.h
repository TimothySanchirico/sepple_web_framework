#include <string>
#include <iostream>


#ifndef PREFIX_TREE
#define PREFIX_TREE

// T must have assignment operator and default constructor
template <typename T>
struct PrefixNode {
    public:
        PrefixNode<T>();
        PrefixNode<T>(T & t);
        PrefixNode<T> * & operator [](size_t i);
        void set_data(T & t);
        T & get_data();

    private:
        T data;
        bool d_set;
        PrefixNode<T> * children[257];
};


template <typename T>
class PrefixTree {
    public:
        PrefixTree<T>();

        void insert(const std::string & key, T n);

        T & get(const std::string & key) const;

        ~PrefixTree<T>();

    private:
        PrefixNode<T> * root;
        void _insert(PrefixNode<T> * r, const std::string &s,
            size_t i, T & n);
        T & _get(PrefixNode<T> * r, const std::string &s, size_t i) const;
        void _delete(PrefixNode<T> * r);
};

#include "prefix_tree.cpp"
#endif

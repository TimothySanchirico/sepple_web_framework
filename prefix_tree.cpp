
// PrefixNode
template <typename T>
PrefixNode<T>::PrefixNode(T & t) : d_set(true), data(t){
    for(size_t i = 0; i < 257;) 
        children[i++] = nullptr;
}

template <typename T>
PrefixNode<T>::PrefixNode() : d_set(false){
    for(size_t i = 0; i < 257;) 
        children[i++] = nullptr;
}

template <typename T>
PrefixNode<T> * & PrefixNode<T>::operator [](size_t i){
    // TODO
    // Check if this is actually faster than a branch
    return children[(i > 256)*256 +(i <=256)*i];
}

template <typename T>
void PrefixNode<T>::set_data(T & t){
    data = t;
}

template <typename T>
T & PrefixNode<T>::get_data(){
    return data;
}


// PrefixTree
template <typename T>
PrefixTree<T>::PrefixTree() {
    root = new PrefixNode<T>();
}

template <typename T>
void PrefixTree<T>::insert(const std::string & s, T n){
    _insert(root, s, 0, n);
}

template <typename T>
void PrefixTree<T>::_insert(PrefixNode<T> * r,
    const std::string & s, size_t i, T & n){
    PrefixNode<T> * c;
    if(i == s.size()-1){
        if((*r)[s[i]]){
            (*r)[s[i]]->set_data(n);
        }
        else{
            c = new PrefixNode<T>(n);
            (*r)[s[i]] = c;
        }
    }
    else {
        if((*r)[s[i]] == nullptr){
            c = new PrefixNode<T>();
            (*r)[s[i]] = c;
        }
        _insert((*r)[s[i]], s, i+1, n);
    }
}


template <typename T>
T & PrefixTree<T>::get(const std::string & key) const {
    return _get(root, key, 0);
}


template<typename T>
T & PrefixTree<T>::_get(PrefixNode<T> * r, const std::string & s, size_t i) const {
    if(i == s.size()-1){
        if((*r)[s[i]] == nullptr)
            return root->get_data();
        return (*r)[s[i]]->has_data()? (*r)[s[i]]->get_data() : root->get_data();
    }
    else{
        if((*r)[s[i]] == nullptr)
            return root->get_data();
        else return _get((*r)[s[i]], s, i+1);
    }
}


template <typename T>
PrefixTree<T>::~PrefixTree(){
    _delete(root);
}


template <typename T>
void PrefixTree<T>::_delete(PrefixNode<T> * r){
    if(r){
        for(size_t i = 0; i < 257; i++){
            _delete((*r)[i]);
        }
        delete r;
    }
}


template <typename T>
void PrefixTree<T>::set_default_data(T n){
    root->set_data(n);
}

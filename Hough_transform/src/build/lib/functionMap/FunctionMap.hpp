#pragma once

#include <vector>
#include <functional>

using namespace std;

template<typename Func, typename object>
class FunctionMapTuple{
    Func key;
    object value;
};

template<typename Func, typename object>
class FunctionMap{
    private:
        vector<FunctionMapTuple<Func, object>> array;
    
    public:
        FunctionMap();
        bool contains(Func key);
        object at(Func key);
        void insert(Func key, object value);
    
    private:
        bool keysAreEqual(Func key1, Func key2);
};
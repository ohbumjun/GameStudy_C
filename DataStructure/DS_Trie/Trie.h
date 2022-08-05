#pragma once

#include <stdlib.h>
#include <cstdlib>
#include <cstring>

struct Trie
{
    bool finish;
    Trie* next[26];

    Trie() : finish(false)
    {
        memset(next, 0, sizeof(next));
    }

    ~Trie()
    {
        for (int i = 0; i < 26; ++i)
        {
            if (next[i])
                delete next[i];
        }
    }

    void insert(const char* key)
    {
        if (*key == '\0')
        {
            finish = true;
        }
        else
        {
            int curr = *key - 'A';

            if (!next[curr])
                next[curr] = new Trie();

            next[curr]->insert(key + 1);
        }
    }

    Trie* find(const char* key)
    {
        if (key == '\0')
            return this;

        int curr = *key - 'A';

        if (next[curr] == nullptr)
            return nullptr;

        return next[curr]->find(key + 1);
    }
};

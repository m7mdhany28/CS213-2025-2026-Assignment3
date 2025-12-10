// DictionaryManager.h
#ifndef DICTIONARY_MANAGER_H
#define DICTIONARY_MANAGER_H

#include <string>
#include <unordered_set>
#include <vector>

class DictionaryManager {
private:
    std::unordered_set<std::string> dictionary;
    static DictionaryManager* instance;
    DictionaryManager(); // Private constructor for singleton

public:
    static DictionaryManager* getInstance();

    void loadDictionary(const std::string& filename = "dic.txt");
    bool isValidWord(const std::string& word) const;
    bool isValidPartialWord(const std::string& prefix) const;
    std::vector<std::string> getAllWords() const;
};

#endif
// DictionaryManager.cpp
#include "DictionaryManager.h"
#include <fstream>
#include <iostream>
#include <algorithm>

DictionaryManager* DictionaryManager::instance = nullptr;

DictionaryManager::DictionaryManager() {
    loadDictionary();
}

DictionaryManager* DictionaryManager::getInstance() {
    if (!instance) {
        instance = new DictionaryManager();
    }
    return instance;
}

void DictionaryManager::loadDictionary(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Warning: Could not open dictionary file: " << filename << std::endl;
        return;
    }

    std::string word;
    while (file >> word) {
        // Convert to uppercase for consistency
        std::transform(word.begin(), word.end(), word.begin(), ::toupper);
        dictionary.insert(word);
    }

    file.close();
    std::cout << "Loaded " << dictionary.size() << " words from dictionary." << std::endl;
}

bool DictionaryManager::isValidWord(const std::string& word) const {
    std::string upperWord = word;
    std::transform(upperWord.begin(), upperWord.end(), upperWord.begin(), ::toupper);
    return dictionary.find(upperWord) != dictionary.end();
}

bool DictionaryManager::isValidPartialWord(const std::string& prefix) const {
    std::string upperPrefix = prefix;
    std::transform(upperPrefix.begin(), upperPrefix.end(), upperPrefix.begin(), ::toupper);

    // Check if any word starts with this prefix
    for (const auto& word : dictionary) {
        if (word.substr(0, upperPrefix.length()) == upperPrefix) {
            return true;
        }
    }
    return false;
}

std::vector<std::string> DictionaryManager::getAllWords() const {
    return std::vector<std::string>(dictionary.begin(), dictionary.end());
}
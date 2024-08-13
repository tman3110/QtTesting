#include"Trie.h"

TrieNode::TrieNode() {
    wordEnd = false;
    for (int i = 0; i < sizeChar; i++) {
        childNode[i] = nullptr;
    }
}

bool TrieNode::isLeaf() const {
    //Leaf has no children
    for (int i = 0; i < sizeChar; i++) {
        if (childNode[i] != nullptr) return false;
    }
    return true;
}

Trie::Trie() {
    root = new TrieNode();
    listHistory = nullptr;
    listFavorite = nullptr;
}

TrieNode* Trie::getRoot() {
    return this->root;
}

TrieNode* Trie::removeHelper(TrieNode* root, wstring key, int depth) {
    if (root == nullptr) return nullptr;

    if (depth == key.size()) {
        root->wordEnd = false;
        listHistory->erase(root->word.key);
        listFavorite->erase(root->word.key);
        // If given is not prefix of any other word
        if (root->isLeaf()) {
            delete root;
            root = nullptr;
        }

        return root;
    }

    int idx = mp[key[depth]];
    root->childNode[idx] = removeHelper(root->childNode[idx], key, depth + 1);


    // If root does not have any child (its only child got deleted), and it is not end of another word.
    if (root->isLeaf() && root->wordEnd == false) {
        delete root;
        root = nullptr;
    }

    return root;
}

void Trie::remove(wstring key) {
    for (int i = 0; i < (int)key.length(); i++) {
        key[i] = lowerCase(key[i]);
    }
    this->root = removeHelper(this->root, key, 0);
}

void Trie::insert(Word word) {
    TrieNode* cur = this->root;
    for (int i = 0; i < (int)word.key.size(); i++) {
        wchar_t c = lowerCase(word.key[i]);
        int idx = mp[c];
        if (idx < 0) continue;
        if (cur->childNode[idx] == nullptr) {
            cur->childNode[idx] = new TrieNode();
        }
        cur = cur->childNode[idx];
    }
    if (cur->wordEnd == false) {
        cur->wordEnd = true;
        cur->word = word;
    }
    else {
        if (cur->word.type.find(word.type) == -1 && !word.type.empty()) {
            if (cur->word.type.empty()) cur->word.type += word.type;
            else cur->word.type += L", " + word.type;
        }
        if (cur->word.spelling.find(word.spelling) == -1 && !word.spelling.empty()) {
            if (cur->word.spelling.empty()) cur->word.spelling += word.spelling;
            else cur->word.spelling += L", " + word.spelling;
        }
        for (int i = 0; i < (int)word.definitions.size(); i++) {
            cur->word.addDefinition(word.definitions[i]);
        }
    }
}

void Trie::saveHelper(TrieNode* root, ofstream& fout) {
    if (root->wordEnd) {
        fout << utf16_to_utf8(root->word.key) << "\n";
        if (root->word.type.empty()) fout << "-\n";
        else fout << utf16_to_utf8(root->word.type) << "\n";
        if (root->word.spelling.empty()) fout << "-\n";
        else fout << utf16_to_utf8(root->word.spelling) << "\n";
        for (int i = 0; i < (int)root->word.definitions.size(); i++) {
            if (i != 0) fout << "|";
            fout << utf16_to_utf8(root->word.definitions[i]);
        }
        fout << "\n\n";
    }

    for (int i = 0; i < sizeChar; i++) {
        if (root->childNode[i]) saveHelper(root->childNode[i], fout);
    }
}

void Trie::save(ofstream& fout) {
    saveHelper(this->root, fout);
    fout.close();
}

void Trie::suggest(TrieNode* node, vector<Word>& list) {
    if (node == nullptr) return;
    if (list.size() >= 10) return;

    if (node->wordEnd) {
        list.push_back(node->word);
    }

    for (int i = 0; i < sizeChar; i++) {
        suggest(node->childNode[i], list);
    }
}

bool Trie::search(Word& word, wstring key, vector<Word>& list) {
    TrieNode* cur = this->root;

    for (int i = 0; i < (int)key.length(); i++) {
        wchar_t c = lowerCase(key[i]);
        int idx = mp[c];
        if (cur->childNode[idx] == nullptr) return false;
        cur = cur->childNode[idx];
    }

    suggest(cur, list);

    if (cur->wordEnd) {
        word = cur->word;
        return true;
    }

    return false;
}

void Trie::clearHelper(TrieNode* root) {
    for (int i = 0; i < sizeChar; i++) {
        if (root->childNode[i]) clearHelper(root->childNode[i]);
    }
    delete root;
}

void Trie::clear() {
    if (root == nullptr) return;
    clearHelper(this->root);
    this->root = nullptr;
    this->listFavorite = nullptr;
    this->listHistory = nullptr;
}

void Trie::setListHistory(List* list) {
    this->listHistory = list;
}

void Trie::setListFavorite(List* list) {
    this->listFavorite = list;
}
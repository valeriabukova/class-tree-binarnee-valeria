//
//  main.cpp
//  homework binarnee tree
//
//  Created by Valeria  Bukova on 05.11.2024.
//

#include <iostream>

const int MAX_LENGTH = 50;

struct Node {
    char englishWord[MAX_LENGTH];
    char russianTranslation[MAX_LENGTH];
    int accessCounter;
    Node* left;
    Node* right;

    Node(const char* eng, const char* rus, int counter = 0)
        : accessCounter(counter), left(nullptr), right(nullptr) {
        for (int i = 0; i < MAX_LENGTH - 1 && eng[i] != '\0'; ++i)
            englishWord[i] = eng[i];
        englishWord[MAX_LENGTH - 1] = '\0';

        for (int i = 0; i < MAX_LENGTH - 1 && rus[i] != '\0'; ++i)
            russianTranslation[i] = rus[i];
        russianTranslation[MAX_LENGTH - 1] = '\0';
    }
};

class BinaryTree {
private:
    Node* root;

    int compareStrings(const char* s1, const char* s2) {
        int i = 0;
        while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i]) {
            ++i;
        }
        return s1[i] - s2[i];
    }

    void copyString(char* dest, const char* src) {
        int i = 0;
        while (src[i] != '\0' && i < MAX_LENGTH - 1) {
            dest[i] = src[i];
            ++i;
        }
        dest[i] = '\0';
    }

    Node* insert(Node* node, const char* eng, const char* rus, int counter = 0) {
        if (!node) {
            return new Node(eng, rus, counter);
        }
        if (compareStrings(eng, node->englishWord) < 0) {
            node->left = insert(node->left, eng, rus, counter);
        } else if (compareStrings(eng, node->englishWord) > 0) {
            node->right = insert(node->right, eng, rus, counter);
        }
        return node;
    }

    Node* find(Node* node, const char* eng) {
        if (!node || compareStrings(node->englishWord, eng) == 0) {
            return node;
        }
        if (compareStrings(eng, node->englishWord) < 0) {
            return find(node->left, eng);
        }
        return find(node->right, eng);
    }

    Node* findMin(Node* node) {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    Node* remove(Node* node, const char* eng) {
        if (!node) return nullptr;

        if (compareStrings(eng, node->englishWord) < 0) {
            node->left = remove(node->left, eng);
        } else if (compareStrings(eng, node->englishWord) > 0) {
            node->right = remove(node->right, eng);
        } else {
            if (!node->left) {
                Node* temp = node->right;
                delete node;
                return temp;
            } else if (!node->right) {
                Node* temp = node->left;
                delete node;
                return temp;
            }

            Node* temp = findMin(node->right);
            copyString(node->englishWord, temp->englishWord);
            copyString(node->russianTranslation, temp->russianTranslation);
            node->accessCounter = temp->accessCounter;
            node->right = remove(node->right, temp->englishWord);
        }
        return node;
    }

    void displayWord(Node* node, const char* eng) {
        Node* found = find(node, eng);
        if (found) {
            found->accessCounter++;
            std::cout << "Word: " << found->englishWord << ", Translation: " << found->russianTranslation << std::endl;
        } else {
            std::cout << "Word not found." << std::endl;
        }
    }

    void findMostLeastPopular(Node* node, Node*& mostPopular, Node*& leastPopular) {
        if (!node) return;

        if (!mostPopular || node->accessCounter > mostPopular->accessCounter) {
            mostPopular = node;
        }
        if (!leastPopular || node->accessCounter < leastPopular->accessCounter) {
            leastPopular = node;
        }

        findMostLeastPopular(node->left, mostPopular, leastPopular);
        findMostLeastPopular(node->right, mostPopular, leastPopular);
    }

    void deleteTree(Node* node) {
        if (node) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

public:
    BinaryTree() : root(nullptr) {}

    ~BinaryTree() {
        deleteTree(root);
    }

    void addWord(const char* eng, const char* rus, int counter = 0) {
        root = insert(root, eng, rus, counter);
    }

    void displayWord(const char* eng) {
        displayWord(root, eng);
    }

    void replaceTranslation(const char* eng, const char* newRus) {
        Node* found = find(root, eng);
        if (found) {
            copyString(found->russianTranslation, newRus);
            std::cout << "Translation updated successfully." << std::endl;
        } else {
            std::cout << "Word not found." << std::endl;
        }
    }

    void removeWord(const char* eng) {
        root = remove(root, eng);
    }

    void showMostPopularWord() {
        Node* mostPopular = nullptr;
        Node* leastPopular = nullptr;
        findMostLeastPopular(root, mostPopular, leastPopular);
        if (mostPopular) {
            std::cout << "Most popular word: " << mostPopular->englishWord
                      << " (Translation: " << mostPopular->russianTranslation
                      << ", Access Count: " << mostPopular->accessCounter << ")" << std::endl;
        } else {
            std::cout << "Dictionary is empty." << std::endl;
        }
    }

    void showLeastPopularWord() {
        Node* mostPopular = nullptr;
        Node* leastPopular = nullptr;
        findMostLeastPopular(root, mostPopular, leastPopular);
        if (leastPopular) {
            std::cout << "Least popular word: " << leastPopular->englishWord
                      << " (Translation: " << leastPopular->russianTranslation
                      << ", Access Count: " << leastPopular->accessCounter << ")" << std::endl;
        } else {
            std::cout << "Dictionary is empty." << std::endl;
        }
    }
};

int main() {
    BinaryTree dictionary;

    dictionary.addWord("hello", "привет", 5);
    dictionary.addWord("world", "мир", 3);
    dictionary.addWord("friend", "друг", 2);

    dictionary.displayWord("hello");

    dictionary.replaceTranslation("world", "земля");

    dictionary.removeWord("friend");

    dictionary.showMostPopularWord();
    dictionary.showLeastPopularWord();

    return 0;
}

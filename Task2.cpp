
#include <iostream>
#include <string>
using namespace std;

struct WordNode {
    string word;
    int frequency;
    WordNode* left;
    WordNode* right;
};

WordNode* insertWord(WordNode* root, string word) {
    if (root == NULL) {
        WordNode* newNode = new WordNode;
        newNode->word = word;
        newNode->frequency = 1;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }

    if (word < root->word)
        root->left = insertWord(root->left, word);
    else if (word > root->word)
        root->right = insertWord(root->right, word);
    else
        root->frequency++;

    return root;
}

void insertParagraph(WordNode*& root, const string& paragraph) {
    string word = "";

    for (int i = 0; i < (int)paragraph.length(); i++) {
        char ch = paragraph[i];

        if (ch >= 'A' && ch <= 'Z')
            ch = ch - 'A' + 'a';

        if (ch >= 'a' && ch <= 'z')
            word += ch;
        else if (word != "") {
            root = insertWord(root, word);
            word = "";
        }
    }

    if (word != "")
        root = insertWord(root, word);
}

void displayAlphabetical(WordNode* root) {
    if (root == NULL)
        return;

    displayAlphabetical(root->left);
    cout << root->word << " : " << root->frequency << endl;
    displayAlphabetical(root->right);
}

int countUniqueWords(WordNode* root) {
    if (root == NULL)
        return 0;

    return 1 + countUniqueWords(root->left)
             + countUniqueWords(root->right);
}

void fillArray(WordNode* root, WordNode* words[], int& index) {
    if (root == NULL)
        return;

    fillArray(root->left, words, index);
    words[index] = root;
    index++;
    fillArray(root->right, words, index);
}

void displayByFrequency(WordNode* root) {
    int total = countUniqueWords(root);

    if (total == 0)
        return;

    WordNode** words = new WordNode*[total];

    int index = 0;
    fillArray(root, words, index);

    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - i - 1; j++) {
            if (words[j]->frequency < words[j + 1]->frequency) {
                WordNode* temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < total; i++)
        cout << words[i]->word << " : "
             << words[i]->frequency << endl;

    delete[] words;
}

void findMaxFrequency(WordNode* root, int& maxFrequency) {
    if (root == NULL)
        return;

    if (root->frequency > maxFrequency)
        maxFrequency = root->frequency;

    findMaxFrequency(root->left, maxFrequency);
    findMaxFrequency(root->right, maxFrequency);
}

void displayMostFrequent(WordNode* root, int maxFrequency) {
    if (root == NULL)
        return;

    displayMostFrequent(root->left, maxFrequency);

    if (root->frequency == maxFrequency)
        cout << root->word << " : " << root->frequency << endl;

    displayMostFrequent(root->right, maxFrequency);
}

void findMostFrequentWord(WordNode* root) {
    if (root == NULL) {
        cout << "No words found.\n";
        return;
    }

    int maxFrequency = 0;
    findMaxFrequency(root, maxFrequency);

    cout << "Most frequent word(s):\n";
    displayMostFrequent(root, maxFrequency);
}

int countTotalWords(WordNode* root) {
    if (root == NULL)
        return 0;

    return root->frequency
         + countTotalWords(root->left)
         + countTotalWords(root->right);
}

void deleteTree(WordNode* root) {
    if (root == NULL)
        return;

    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    string paragraph =
        "The quick brown fox jumps over the lazy dog. The fox is quick and smart.";

    WordNode* root = NULL;

    insertParagraph(root, paragraph);

    cout << "Words in Alphabetical Order:\n";
    displayAlphabetical(root);

    cout << "\nWords by Frequency (Highest to Lowest):\n";
    displayByFrequency(root);

    cout << endl;
    findMostFrequentWord(root);

    cout << "\nTotal Unique Words: "
         << countUniqueWords(root) << endl;

    cout << "Total Word Count: "
         << countTotalWords(root) << endl;

    deleteTree(root);

    return 0;
}

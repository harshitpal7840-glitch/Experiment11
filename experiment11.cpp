#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>
using namespace std;

// Node of Huffman Tree
struct Node {
    char ch;
    int freq;

    Node* left;
    Node* right;

    Node(char c, int f) {
        ch = c;
        freq = f;
        left = nullptr;
        right = nullptr;
    }
};

// Comparator for Min Heap
struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

// Generate Huffman Codes
void generateCodes(Node* root, string code,
                   unordered_map<char, string>& huffmanCode) {

    if (root == nullptr)
        return;

    // Leaf node
    if (root->left == nullptr && root->right == nullptr) {
        huffmanCode[root->ch] = code;
        return;
    }

    // Left = 0
    generateCodes(root->left, code + "0", huffmanCode);

    // Right = 1
    generateCodes(root->right, code + "1", huffmanCode);
}

// Delete Huffman Tree
void deleteTree(Node* root) {
    if (root == nullptr)
        return;

    deleteTree(root->left);
    deleteTree(root->right);

    delete root;
}

int main() {

    string text;

    cout << "Enter the text: ";
    getline(cin, text);

    if (text.empty()) {
        cout << "Input text is empty.\n";
        return 0;
    }

    // Count frequency of each character
    unordered_map<char, int> frequency;

    for (char ch : text) {
        frequency[ch]++;
    }

    // Min Heap
    priority_queue<Node*, vector<Node*>, Compare> minHeap;

    // Create nodes for each character
    for (auto pair : frequency) {
        minHeap.push(new Node(pair.first, pair.second));
    }

    // Build Huffman Tree
    while (minHeap.size() > 1) {

        Node* left = minHeap.top();
        minHeap.pop();

        Node* right = minHeap.top();
        minHeap.pop();

        Node* newNode = new Node(
            '\0',
            left->freq + right->freq
        );

        newNode->left = left;
        newNode->right = right;

        minHeap.push(newNode);
    }

    // Root of Huffman Tree
    Node* root = minHeap.top();

    // Generate Huffman Codes
    unordered_map<char, string> huffmanCode;

    generateCodes(root, "", huffmanCode);

    // Display frequencies and codes
    cout << "\nCharacter\tFrequency\tHuffman Code\n";
    cout << "---------------------------------------------\n";

    for (auto pair : frequency) {

        char ch = pair.first;

        if (ch == ' ')
            cout << "Space";
        else
            cout << ch;

        cout << "\t\t" << pair.second
             << "\t\t" << huffmanCode[ch] << endl;
    }

    // Encode the original text
    string encodedText = "";

    for (char ch : text) {
        encodedText += huffmanCode[ch];
    }

    cout << "\nOriginal Text:\n";
    cout << text << endl;

    cout << "\nEncoded Text:\n";
    cout << encodedText << endl;

    cout << "\nOriginal Size: "
         << text.length() * 8
         << " bits";

    cout << "\nCompressed Size: "
         << encodedText.length()
         << " bits";

    cout << "\n";

    deleteTree(root);

    return 0;
}
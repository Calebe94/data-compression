#include <SPIFFS.h>
#include <vector>
#include <queue>
#include <bitset>
#include "huffman_coding.h"
// Definição de um nó da árvore de Huffman
struct HuffmanCompression::HuffmanNode {
  char data;
  int frequency;
  HuffmanNode* left;
  HuffmanNode* right;

  HuffmanNode(char d, int freq) : data(d), frequency(freq), left(nullptr), right(nullptr) {}
};

// Comparador personalizado para a fila de prioridade
struct HuffmanCompression::CompareHuffmanNodes {
  bool operator()(HuffmanNode* a, HuffmanNode* b) {
    return a->frequency > b->frequency;
  }
};

HuffmanCompression::HuffmanCompression() {
}

// Função para construir a árvore de Huffman a partir dos dados
HuffmanCompression::HuffmanNode* HuffmanCompression::buildHuffmanTree(std::map<char, int>& frequencies) {
  std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareHuffmanNodes> pq;

  // Inicializa a fila de prioridade com os nós iniciais
  for (auto pair : frequencies) {
    pq.push(new HuffmanNode(pair.first, pair.second));
  }

  // Constrói a árvore de Huffman
  while (pq.size() > 1) {
    HuffmanNode* left = pq.top();
    pq.pop();
    HuffmanNode* right = pq.top();
    pq.pop();

    HuffmanNode* newNode = new HuffmanNode('$', left->frequency + right->frequency);
    newNode->left = left;
    newNode->right = right;

    pq.push(newNode);
  }

  return pq.top();
}

// Função para construir a tabela de Huffman
void HuffmanCompression::buildHuffmanTable(HuffmanNode* root, String code, std::map<char, String>& huffmanTable) {
  if (!root)
    return;

  if (root->data != '$') {
    huffmanTable[root->data] = code;
  }

  buildHuffmanTable(root->left, code + "0", huffmanTable);
  buildHuffmanTable(root->right, code + "1", huffmanTable);
}

// Função para comprimir dados usando Huffman Coding
String HuffmanCompression::compress(const String& input, std::map<char, String>& huffmanTable) {
  String compressedData = "";

  for (char c : input) {
    compressedData += huffmanTable[c];
  }

  return compressedData;
}

// Função para descomprimir dados usando Huffman Coding
String HuffmanCompression::decompress(const String& compressedData, HuffmanNode* root) {
  String decompressedData = "";
  HuffmanNode* current = root;

  for (char bit : compressedData) {
    if (bit == '0') {
      current = current->left;
    } else if (bit == '1') {
      current = current->right;
    }

    if (current->data != '$') {
      decompressedData += current->data;
      current = root;
    }
  }

  return decompressedData;
}

void HuffmanCompression::compressAndDecompress(const String& input) {
  std::map<char, int> frequencies;
  for (char c : input) {
    frequencies[c]++;
  }

  HuffmanNode* root = buildHuffmanTree(frequencies);
  std::map<char, String> huffmanTable;
  buildHuffmanTable(root, "", huffmanTable);

  String compressedData = compress(input.c_str(), huffmanTable);
  Serial.println("Dados comprimidos:");
  Serial.println(compressedData.c_str());

  // Descomprimir dados
  String decompressedData = decompress(compressedData, root);
  Serial.println("Dados descomprimidos:");
  Serial.println(decompressedData.c_str());
}

#ifndef HUFFMAN_CONDIG_H_
#define HUFFMAN_CONDIG_H_

#include <Arduino.h>
#include <map>
#include <string>

class HuffmanCompression {
 public:
  HuffmanCompression();
  void compressAndDecompress(const String &input);

 private:
  struct HuffmanNode;
  struct CompareHuffmanNodes;
  HuffmanNode *buildHuffmanTree(std::map<char, int> &frequencies);
  void buildHuffmanTable(HuffmanNode *root, String code, std::map<char, String> &huffmanTable);
  String compress(const String &input, std::map<char, String> &huffmanTable);
  String decompress(const String &compressedData, HuffmanNode *root);
};
#endif

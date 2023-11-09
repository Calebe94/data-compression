#include "lzw.h"

std::vector<String> dictionary;

static void initDictionary() {
  dictionary.clear();
  for (int i = 0; i < 256; i++) {
    dictionary.push_back(String(1, (char)i));
  }
}

static int findInDictionary(const String &sequence) {
  for (int i = 0; i < dictionary.size(); i++) {
    if (dictionary[i] == sequence) {
      return i;
    }
  }
  return -1;
}

static void addToDictionary(const String &sequence) {
  dictionary.push_back(sequence);
}

void lzw_compress(const String &input) {
  initDictionary();
  String sequence = "";
  std::vector<int> compressedData;

  for (char c : input) {
    String newSequence = sequence + c;
    int index = findInDictionary(newSequence);

    if (index != -1) {
      sequence = newSequence;
    } else {
      compressedData.push_back(findInDictionary(sequence));
      addToDictionary(newSequence);
      sequence = String(1, c);
    }
  }

  if (!sequence.isEmpty()) {
    compressedData.push_back(findInDictionary(sequence));
  }

  for (int value : compressedData) {
    Serial.print(value);
    Serial.print(" ");
  }
}

String lzw_decompress(const std::vector<int>& compressedData) {
  initDictionary();
  String decompressedData = "";
  String sequence = dictionary[compressedData[0]];

  for (int i = 1; i < compressedData.size(); i++) {
    int currentCode = compressedData[i];

    if (currentCode >= dictionary.size()) {
      String currentSequence = sequence + sequence[0];
      dictionary.push_back(currentSequence);
      decompressedData += sequence;
      sequence = currentSequence;
    } else {
      decompressedData += dictionary[currentCode];
      String currentSequence = sequence + dictionary[currentCode][0];
      dictionary.push_back(currentSequence);
      sequence = currentSequence;
    }
  }

  return decompressedData;
}

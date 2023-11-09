#include "rle.h"

String rle_compress(const String &input) {
  String compressedData = "";
  int length = input.length();
  int count = 1;

  for (int i = 0; i < length; i++) {
    if (i < length - 1 && input[i] == input[i + 1]) {
      count++;
    } else {
      compressedData += input[i];
      compressedData += count;
      count = 1;
    }
  }

  return compressedData;
}

String rle_decompress(const String &compressedData) {
  String decompressedData = "";
  int length = compressedData.length();

  for (int i = 0; i < length; i += 2) {
    char character = compressedData[i];
    int count = compressedData[i + 1] - '0';

    for (int j = 0; j < count; j++) {
      decompressedData += character;
    }
  }

  return decompressedData;
}

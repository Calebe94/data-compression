#ifndef lzw_h
#define lzw_h

#include <Arduino.h>
#include <vector>
#include <string>

void lzw_compress(const String& input);

String lzw_decompress(const std::vector<int>& compressedData);

#endif

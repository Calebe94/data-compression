#ifndef RLE_H_
#define RLE_H_

#include <Arduino.h>

String rle_compress(const String &input);
String rle_decompress(const String &compressedData);

#endif // RLE_H_

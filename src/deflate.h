#ifndef DEFLATE_H_
#define DEFLATE_HL_

bool deflate_compress(const String& inputFilePath, const String& outputFilePath);
bool deflate_decompress(const String& inputFilePath, const String& outputFilePath);

#endif

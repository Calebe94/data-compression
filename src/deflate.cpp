#include <Arduino.h>
#include <SPIFFS.h>
#include <zlib.h>
#include "deflate.h"

bool deflate_compress(const String& inputFilePath, const String& outputFilePath) {
  File inputFile = SPIFFS.open(inputFilePath, "r");
  File outputFile = SPIFFS.open(outputFilePath, "w");

  if (!inputFile || !outputFile) {
    return false;
  }

  const size_t bufferSize = 1024;
  uint8_t inputBuffer[bufferSize];
  uint8_t outputBuffer[bufferSize];

  z_stream stream;
  stream.zalloc = Z_NULL;
  stream.zfree = Z_NULL;
  stream.opaque = Z_NULL;
  stream.avail_in = 0;
  stream.next_in = Z_NULL;
  if (deflateInit(&stream, Z_BEST_COMPRESSION) != Z_OK) {
    return false;
  }

  int flush;
  do {
    stream.avail_in = inputFile.read(inputBuffer, bufferSize);
    if (stream.avail_in == 0) {
      break;
    }
    stream.next_in = inputBuffer;
    flush = (stream.avail_in == 0) ? Z_FINISH : Z_NO_FLUSH;
    stream.avail_out = bufferSize;
    stream.next_out = outputBuffer;

    if (deflate(&stream, flush) == Z_STREAM_ERROR) {
      return false;
    }

    outputFile.write(outputBuffer, bufferSize - stream.avail_out);
  } while (flush != Z_FINISH);

  deflateEnd(&stream);
  inputFile.close();
  outputFile.close();

  return true;
}

bool deflate_decompress(const String& inputFilePath, const String& outputFilePath) {
  File inputFile = SPIFFS.open(inputFilePath, "r");
  File outputFile = SPIFFS.open(outputFilePath, "w");

  if (!inputFile || !outputFile) {
    return false;
  }

  const size_t bufferSize = 1024;
  uint8_t inputBuffer[bufferSize];
  uint8_t outputBuffer[bufferSize];

  z_stream stream;
  stream.zalloc = Z_NULL;
  stream.zfree = Z_NULL;
  stream.opaque = Z_NULL;
  stream.avail_in = 0;
  stream.next_in = Z_NULL;
  if (inflateInit(&stream) != Z_OK) {
    return false;
  }

  int ret;
  do {
    stream.avail_in = inputFile.read(inputBuffer, bufferSize);
    if (stream.avail_in == 0) {
      break;
    }
    stream.next_in = inputBuffer;

    do {
      stream.avail_out = bufferSize;
      stream.next_out = outputBuffer;
      ret = inflate(&stream, Z_NO_FLUSH);

      if (ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
        inflateEnd(&stream);
        return false;
      }

      outputFile.write(outputBuffer, bufferSize - stream.avail_out);
    } while (stream.avail_out == 0);

  } while (ret != Z_STREAM_END);

  inflateEnd(&stream);
  inputFile.close();
  outputFile.close();

  return true;
}

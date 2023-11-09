#include <Arduino.h>
#include <SPIFFS.h>
#include <vector>
#ifdef RLE
#include "rle.h"
#elif HUFFMAN_CODE
#include "huffman_coding.h"
#elif DEFLATE
#include "deflate.h"
#elif ZIP
#include "zip.h"
#elif LZW
#include "lzw.h"
#endif

void setup() {
  Serial.begin(115200);

  if (!SPIFFS.begin(true)) {
    Serial.println("Falha ao montar a memória flash SPIFFS");
    return;
  }

  File dataFile = SPIFFS.open("/data.csv", "r");
  if (!dataFile) {
    Serial.println("Falha ao abrir o arquivo CSV na memória flash");
    return;
  }

  String input = "";
  while (dataFile.available()) {
    char c = dataFile.read();
    input += c;
  }

  dataFile.close();

#ifdef RLE
  Serial.println("Dados originais:");
  Serial.println(input);

  // Comprimir dados
  String compressedData = rle_compress(input.c_str());
  Serial.println("Dados comprimidos:");
  Serial.println(compressedData.c_str());

  Descomprimir dados
  String decompressedData = rle_decompress(compressedData.c_str());
  Serial.println("Dados descomprimidos:");
  Serial.println(decompressedData.c_str());
#elif HUFFMAN_CODE
  HuffmanCompression huffman;
  huffman.compressAndDecompress(input);
#elif DEFLATE
  String inputFilePath = "/data.csv";
  String compressedFilePath = "/compressed_data.deflate";
  String decompressedFilePath = "/decompressed_data.csv";

  if (deflate_compress(inputFilePath, compressedFilePath)) {
    Serial.println("Arquivo comprimido com sucesso.");
    if (deflate_decompress(compressedFilePath, decompressedFilePath)) {
      Serial.println("Arquivo descomprimido com sucesso.");
    } else {
      Serial.println("Falha na descompressão do arquivo.");
    }
  } else {
    Serial.println("Falha na compressão do arquivo.");
  }
#elif LZW
  lzw_compress(input);
#endif
}

void loop() {
  // Nada a fazer aqui
}

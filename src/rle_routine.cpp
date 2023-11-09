#include <Arduino.h>
#include <SPIFFS.h>
#include "rle.h"
#include "rle_routine.h"

void rle_routine()
{
    String inputFilePath = "/dados.csv";
    String compressedFilePath = "/dados_rle_compressed.csv";
    String decompressedFilePath = "/dados_rle_decompressed.csv";

    // Ler os dados originais
    File dataFile = SPIFFS.open(inputFilePath, "r");
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

    // Imprimir o tamanho do arquivo "dados.csv" antes da compressão
    Serial.print("Tamanho do arquivo 'dados.csv' antes da compressão: ");
    Serial.println(input.length());

    // Medir o tempo de compressão
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Comprimir os dados
    String compressedData = rle_compress(input.c_str());

    clock_gettime(CLOCK_MONOTONIC, &end);
    unsigned long long elapsed = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
    Serial.print("Tempo de compressão: ");
    Serial.print(elapsed);
    Serial.println(" micros");

    // Gravar os dados comprimidos em um arquivo chamado "dados_rle_compressed.csv"
    File compressedFile = SPIFFS.open(compressedFilePath, "w");
    if (compressedFile) {
        compressedFile.print(compressedData);
        compressedFile.close();
    } else {
        Serial.println("Falha ao criar o arquivo comprimido");
    }

    // Imprimir o tamanho do arquivo "dados_rle_compressed.csv"
    File compressedSize = SPIFFS.open(compressedFilePath, "r");
    Serial.print("Tamanho do arquivo 'dados_rle_compressed.csv': ");
    Serial.println(compressedSize.size());

    // Medir o tempo de descompressão
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Descomprimir os dados
    String decompressedData = rle_decompress(compressedData.c_str());

    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
    Serial.print("Tempo de descompressão: ");
    Serial.print(elapsed);
    Serial.println(" micros");

    // Gravar os dados descomprimidos em um arquivo chamado "dados_rle_decompressed.csv"
    File decompressedFile = SPIFFS.open(decompressedFilePath, "w");
    if (decompressedFile) {
        decompressedFile.print(decompressedData);
        decompressedFile.close();
    } else {
        Serial.println("Falha ao criar o arquivo descomprimido");
    }

    // Imprimir o tamanho do arquivo "dados_rle_decompressed.csv"
    File decompressedSize = SPIFFS.open(decompressedFilePath, "r");
    Serial.print("Tamanho do arquivo 'dados_rle_decompressed.csv': ");
    Serial.println(decompressedSize.size());
}

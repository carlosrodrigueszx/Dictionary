#ifndef TEXTPROCESSOR_HPP
#define TEXTPROCESSOR_HPP

#include <string>
#include <vector>

// Limpa uma palavra: remove pontuações e converte para minúsculas
std::string cleanWord(const std::string& raw);

// Lê um arquivo .txt e retorna as palavras processadas
std::vector<std::string> readAndProcessText(const std::string& filepath);

#endif

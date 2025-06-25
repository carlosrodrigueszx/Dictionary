#include "TextProcessor.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

// Lista de letras válidas (acentuadas + padrão ASCII)
const std::string validChars =
    "abcdefghijklmnopqrstuvwxyz"
    "áàâãéêíóôõúç"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "ÁÀÂÃÉÊÍÓÔÕÚÇ";

// Testa se o caractere faz parte de uma letra válida (com ou sem acento)
bool isLetter(char c) {
    return validChars.find(c) != std::string::npos;
}

// Converte caractere para minúsculo com segurança
char toLowerSafe(char c) {
    return std::tolower(static_cast<unsigned char>(c));
}

// Limpa a palavra: remove pontuação e mantém hífen apenas entre letras
std::string cleanWord(const std::string& raw) {
    std::string result;
    for (size_t i = 0; i < raw.size(); ++i) {
        char ch = raw[i];

        if (isLetter(ch)) {
            result += toLowerSafe(ch);
        } else if (ch == '-') {
            // Mantém o hífen apenas se estiver entre letras
            if (i > 0 && i + 1 < raw.size() &&
                isLetter(raw[i - 1]) &&
                isLetter(raw[i + 1])) {
                result += '-';
            }
        }
    }
    return result;
}

// Lê e processa o texto
std::vector<std::string> readAndProcessText(const std::string& filepath) {
    std::ifstream file(filepath);
    std::vector<std::string> words;
    std::string line;

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string word;
        while (ss >> word) {
            std::string cleaned = cleanWord(word);
            if (!cleaned.empty()) {
                words.push_back(cleaned);
            }
        }
    }

    return words;
}

//
// Created by Matq on 15/01/2025.
//

#include "Words.h"
#include <fstream>

Words::Words() {
    std::ifstream file("assets/words");
    if (!file)
        throw std::runtime_error("[Words.h] words file could not be opened");

    std::string line;
    while (std::getline(file, line))
        word_list.push_back(line);

    file.close();
    std::wcout << Strings::FStringColorsW(L"[Words] &5Loaded %zu words\n", word_list.size());
}

std::string Words::GetRandomWord() const {
    return word_list[rand() % word_list.size()];
}

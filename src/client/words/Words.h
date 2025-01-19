//
// Created by Matq on 15/01/2025.
//

#pragma once

#include <vector>
#include "../../shared/core/Strings.h"

class Words {
private:
    std::vector<std::string> word_list;

public:
    Words();

    // Getting
    [[nodiscard]] std::string GetRandomWord() const;

};

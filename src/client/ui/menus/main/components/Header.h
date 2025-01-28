//
// Created by Matq on 26/01/2025.
//

#pragma once

#include "../../../components/element/Frame.h"
#include "Settings.h"

namespace Main {
class Header : public Frame {
private:

public:
    Header(Settings* settings);
    ~Header();

};
}

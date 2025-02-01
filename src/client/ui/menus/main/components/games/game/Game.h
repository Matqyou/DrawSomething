//
// Created by Matq on 26/01/2025.
//

#pragma once

#include "../../../../../../game/game_info/GameInfo.h"
#include "../../../../../components/element/TextElement.h"
#include "../../../../../components/element/Frame.h"
#include "../../../../../components/element/Button.h"
#include "Statistics.h"
#include "../../../../../CommonUI.h"

namespace Main {
class Games;
class Game : public Frame {
private:
    Statistics* statistics;
    TextElement* turn_number;
    Frame* profile_picture;
    TextElement* picture_text;
    TextElement* state_text;
    TextElement* state_description;
    Button* game_button;
    GameInfo* game_info_;
    Games* games_;

    static LinkTexture sTextureDefaultProfilePicture;

    void SetTurnNumber(int turn);
    void SetProfilePicture(TextureData* texture_data);
    void SetUsername(const char* username);
    void SetState(GameInfo::GameState game_state);

public:
    explicit Game(Games* games);
    ~Game() override;

    // Getting
    [[nodiscard]] GameInfo* GetGameInfo() const { return game_info_; }

    // Manipulation
    void UpdateInfo(GameInfo* game_info);

};
}

//
// Created by Matq on 13/01/2025.
//

#include "IngameToolSelector.h"
#include "../../../../core/Application.h"
#include "../../../components/element/Button.h"

PreloadTexture IngameToolSelector::sTextureOutline("game.tool_selector.outline");
PreloadTexture IngameToolSelector::sTextureOutlineSelected("game.tool_selector.outline_selected");
PreloadTexture IngameToolSelector::sTexturePencil("game.tool_selector.pencil");
PreloadTexture IngameToolSelector::sTextureEraser("game.tool_selector.eraser");
PreloadTexture IngameToolSelector::sTextureDoneButton("game.tool_selector.done_button");

IngameToolSelector::IngameToolSelector()
    : Frame(Vec2i(0, 0), Vec2i(0, 74), DONT_DRAW) {
    auto drawing = Application::Get()->GetDrawing();

    // Tools
    std::vector<Element*> tools;
    auto pencil_tool = (new Frame(Vec2i(0, 0),
                                Vec2i(54, 54),
                                sTextureOutline.GetTexture()))
        ->SetAlign(DONT_ALIGN, ALIGN_CENTER)
        ->SetName("PencilTool", false);
    tools.push_back(pencil_tool);

    auto eraser_tool = (new Frame(Vec2i(0, 0),
                                  Vec2i(54, 54),
                                  sTextureOutlineSelected.GetTexture()))
        ->SetAlign(DONT_ALIGN, ALIGN_CENTER)
        ->SetName("EraserTool", false);
    tools.push_back(eraser_tool);

    auto done_button = (new Button(Vec2i(0, 0),
                                  Vec2i(192, 50),
                                  sTextureDoneButton.GetTexture()))
        ->SetAlign(DONT_ALIGN, ALIGN_CENTER)
        ->SetName("DoneButton", false);
    tools.push_back(done_button);

    // Tool selector
    SetFullyOccupy(true, false);
    SetFlex(FLEX_WIDTH);
    SetName("BrushSelector", false);
    AddChildren({ tools });
}
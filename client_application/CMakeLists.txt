cmake_minimum_required(VERSION 3.29)
project(DrawSomething)

set(CMAKE_CXX_STANDARD 20)

include_directories(
        C:/libs/SDL/include
        C:/libs/SDL_image/include
        C:/libs/SDL_mixer/include
        C:/libs/SDL_ttf/include
        C:/libs/asio/asio/include
        C:/libs/curl/include
        C:/libs/json/include
)

# Detect if we're building in Debug or Release mode
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(SDL_LIB_DIR C:/libs/SDL/cmake-build-debug)
    set(SDL_IMAGE_LIB_DIR C:/libs/SDL_image/cmake-build-debug)
    set(SDL_MIXER_LIB_DIR C:/libs/SDL_mixer/cmake-build-debug)
    set(SDL_TTF_LIB_DIR C:/libs/SDL_ttf/cmake-build-debug)
else()
    set(SDL_LIB_DIR C:/libs/SDL/cmake-build-release)
    set(SDL_IMAGE_LIB_DIR C:/libs/SDL_image/cmake-build-release)
    set(SDL_MIXER_LIB_DIR C:/libs/SDL_mixer/cmake-build-release)
    set(SDL_TTF_LIB_DIR C:/libs/SDL_ttf/cmake-build-release)
endif()

# Add the chosen directories
link_directories(
        ${SDL_LIB_DIR}
        ${SDL_IMAGE_LIB_DIR}
        ${SDL_MIXER_LIB_DIR}
        ${SDL_TTF_LIB_DIR}
        C:/libs/curl/lib
)

#link_directories(
#        C:/libs/SDL/cmake-build-debug
#        C:/libs/SDL_image/cmake-build-debug
#        C:/libs/SDL_mixer/cmake-build-debug
#        C:/libs/SDL_ttf/cmake-build-debug
#        C:/libs/curl/lib
#)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

add_executable(DrawSomething src/main.cpp
        res/client.rc
        src/core/Strings.cpp
        src/core/app/Assets.cpp
        src/core/app/Assets.h
        src/core/app/Clock.cpp
        src/core/app/Clock.h
        src/core/app/Drawing.cpp
        src/core/app/Drawing.h
        src/core/Application.cpp
        src/core/Application.h
        src/ui/menus/intermission/components/Scribbles.cpp
        src/ui/menus/intermission/components/Scribbles.h
        src/ui/components/element/Button.h
        src/ui/components/element/TextBox.cpp
        src/ui/components/element/base/Element.cpp
        src/ui/components/element/base/Element.h
        src/ui/components/element/Button.cpp
        src/ui/components/element/TextBox.h
        src/ui/components/event/EventContext.h
        src/ui/components/element/Frame.cpp
        src/ui/components/element/Frame.h
        src/ui/menus/main/MainMenu.cpp
        src/ui/menus/main/MainMenu.h
        src/core/Rectangles.h
        src/ui/menus/ingame/components/Canvas.cpp
        src/ui/menus/ingame/components/Canvas.h
        src/ui/menus/ingame/IngameMenu.cpp
        src/ui/menus/ingame/IngameMenu.h
        src/ui/menus/FullscreenMenu.cpp
        src/ui/menus/FullscreenMenu.h
        src/ui/menus/ingame/components/panel/Panel.cpp
        src/ui/menus/ingame/components/panel/Panel.h
        src/ui/menus/ingame/components/Header.cpp
        src/ui/menus/ingame/components/Header.h
        src/ui/menus/ingame/components/color_selector/ColorSelector.cpp
        src/ui/menus/ingame/components/color_selector/ColorSelector.h
        src/ui/menus/ingame/components/tool_selector/ToolSelector.cpp
        src/ui/menus/ingame/components/tool_selector/ToolSelector.h
        src/ui/components/element/OverlayButton.cpp
        src/ui/components/element/OverlayButton.h
        src/ui/menus/ingame/components/tool_selector/BrushSizeButton.cpp
        src/ui/menus/ingame/components/tool_selector/BrushSizeButton.h
        src/core/Numbers.h
        src/ui/menus/auth/AuthMenu.cpp
        src/ui/menus/auth/AuthMenu.h
        src/ui/cursors/Circular.cpp
        src/ui/cursors/Circular.h
        src/ui/cursors/Cursors.h
        src/ui/cursors/Cursors.cpp
        src/ui/structures/WrappedText.cpp
        src/ui/structures/WrappedText.h
        src/ui/structures/window_texture/WindowTexture.cpp
        src/ui/structures/window_texture/WindowTexture.h
        src/core/Colors.cpp
        src/core/Colors.h
        src/ui/structures/window_texture/Sheet.cpp
        src/ui/structures/window_texture/Sheet.h
        src/ui/menus/ingame/components/panel/GuessingBar.cpp
        src/ui/menus/ingame/components/panel/GuessingBar.h
        src/ui/menus/ingame/components/panel/Letters.cpp
        src/ui/menus/ingame/components/panel/Letters.h
        src/ui/menus/ingame/components/panel/Letter.cpp
        src/ui/menus/ingame/components/panel/Letter.h
        src/ui/menus/ingame/components/panel/LetterSlot.cpp
        src/ui/menus/ingame/components/panel/LetterSlot.h
        src/ui/menus/intermission/IntermissionMenu.cpp
        src/ui/menus/intermission/IntermissionMenu.h
        src/ui/CommonUI.h
        src/ui/CommonUI.cpp
        src/ui/components/element/TextElement.cpp
        src/ui/components/element/TextElement.h
        src/ui/menus/intermission/components/MiniProfile.cpp
        src/ui/menus/intermission/components/MiniProfile.h
        src/ui/structures/visual_texture/VisualTexture.cpp
        src/ui/structures/visual_texture/VisualTexture.h
        src/ui/menus/main/components/Header.cpp
        src/ui/menus/main/components/MainHeader.h
        src/ui/menus/main/components/Profile.cpp
        src/ui/menus/main/components/Profile.h
        src/ui/menus/main/components/games/CurrentGames.cpp
        src/ui/menus/main/components/games/CurrentGames.h
        src/ui/menus/main/components/Connections.cpp
        src/ui/menus/main/components/Connections.h
        src/ui/menus/main/components/games/game/Game.cpp
        src/ui/menus/main/components/games/game/Game.h
        src/ui/menus/main/components/games/game/Statistics.cpp
        src/ui/menus/main/components/games/game/Statistics.h
        src/ui/menus/picking/PickingMenu.cpp
        src/ui/menus/picking/PickingMenu.h
        src/ui/menus/picking/components/Header.cpp
        src/ui/menus/picking/components/Header.h
        src/ui/menus/main/components/SettingsScreen.cpp
        src/ui/menus/main/components/SettingsScreen.h
        src/ui/menus/loading/LoadingScreen.cpp
        src/ui/menus/loading/LoadingScreen.h
        src/game/game_info/GameInfo.cpp
        src/game/game_info/GameInfo.h
        src/game/game_info/GameStats.cpp
        src/game/game_info/GameStats.h
        src/ui/menus/ingame/components/tool_selector/DrawingToolButton.cpp
        src/ui/menus/ingame/components/tool_selector/DrawingToolButton.h
        src/ui/menus/confirmation/ConfirmationScreen.cpp
        src/ui/menus/confirmation/ConfirmationScreen.h
        src/ui/particles/Particles.cpp
        src/ui/particles/Particles.h
        src/ui/particles/particle/FlyingParticle.cpp
        src/ui/particles/particle/FlyingParticle.h
        src/core/Rectangles.inl
        src/ui/components/element/ScrollFrame.cpp
        src/ui/components/element/ScrollFrame.h
        src/ui/components/element/CheckBox.cpp
        src/ui/components/element/CheckBox.h
        src/ui/components/element/Scrollable.cpp
        src/ui/components/element/Scrollable.h
        src/ui/structures/visual_texture/files/HitboxFile.cpp
        src/ui/structures/visual_texture/files/HitboxFile.h
        src/ui/menus/auth/components/ScrollingBackground.cpp
        src/ui/menus/auth/components/ScrollingBackground.h
        src/network/NetworkClient.cpp
        src/network/NetworkClient.h
        src/ui/menus/picking/components/WordOption.cpp
        src/ui/menus/picking/components/WordOption.h
        src/game/account/Account.cpp
        src/game/account/Account.h
        src/game/GameData.cpp
        src/game/GameData.h
        src/ui/menus/main/admin/AdminScreen.cpp
        src/ui/menus/main/admin/AdminScreen.h
        src/ui/menus/main/admin/UserRecord.cpp
        src/ui/menus/main/admin/UserRecord.h
        src/ui/menus/main/shop/ShopScreen.cpp
        src/ui/menus/main/shop/ShopScreen.h
        src/ui/menus/main/shop/bundle/ColorBundle.cpp
        src/ui/menus/main/shop/bundle/ColorBundle.h
        src/ui/menus/main/shop/bundle/ColorView.h
        src/ui/menus/main/shop/bundle/ColorView.cpp
        src/ui/RenderPresets.h
        src/ui/RenderPresets.cpp
        src/ui/menus/main/profile/ProfileScreen.h
        src/ui/menus/main/profile/ProfileScreen.cpp
        src/ui/menus/ScreenMenu.h
        src/ui/menus/ScreenMenu.cpp
        src/windows_system/WindowsSystem.cpp
        src/ui/menus/main/games/GamesScreen.h
        src/ui/menus/main/games/GamesScreen.cpp
        src/game/account/OwnedColor.h
        src/game/account/OwnedColor.cpp
        src/ui/menus/HeaderFullscreenMenu.h
        src/ui/menus/HeaderFullscreenMenu.cpp
        src/ui/menus/export/ExportMenu.h
        src/ui/menus/export/ExportMenu.cpp
        src/ui/menus/main/shop/bombs/BombOffer.cpp
        src/ui/menus/main/shop/bombs/BombOffer.h
        src/ui/menus/main/shop/bombs/BombOffer.h
)

add_executable(DrawSomethingEditor src/editor.cpp
        res/client.rc
        src/core/Strings.cpp
        src/core/app/Assets.cpp
        src/core/app/Assets.h
        src/core/app/Clock.cpp
        src/core/app/Clock.h
        src/core/app/Drawing.cpp
        src/core/app/Drawing.h
        src/core/Application.cpp
        src/core/Application.h
        src/ui/components/element/Button.cpp
        src/ui/components/element/Button.h
        src/ui/components/element/TextBox.cpp
        src/ui/components/element/TextBox.h
        src/ui/components/element/base/Element.cpp
        src/ui/components/element/base/Element.h
        src/ui/components/event/EventContext.h
        src/ui/components/element/Frame.cpp
        src/ui/components/element/Frame.h
        src/core/Rectangles.h
        src/ui/menus/FullscreenMenu.cpp
        src/ui/menus/FullscreenMenu.h
        src/ui/components/element/OverlayButton.cpp
        src/ui/components/element/OverlayButton.h
        src/ui/cursors/Circular.cpp
        src/ui/cursors/Circular.h
        src/ui/cursors/Cursors.h
        src/ui/cursors/Cursors.cpp
        src/ui/structures/WrappedText.cpp
        src/ui/structures/WrappedText.h
        src/ui/structures/window_texture/WindowTexture.cpp
        src/ui/structures/window_texture/WindowTexture.h
        src/ui/structures/window_texture/Sheet.cpp
        src/ui/structures/window_texture/Sheet.h
        src/ui/CommonUI.h
        src/ui/CommonUI.cpp
        src/ui/components/element/TextElement.cpp
        src/ui/components/element/TextElement.h
        src/ui/structures/visual_texture/VisualTexture.cpp
        src/ui/structures/visual_texture/VisualTexture.h
        src/editor/ui/menus/editor/Interface.cpp
        src/editor/ui/menus/editor/Interface.h
        src/core/Numbers.h
        src/core/Colors.cpp
        src/core/Colors.h
        src/editor/ui/menus/editor/components/texture_button/TextureButton.cpp
        src/editor/ui/menus/editor/components/texture_button/TextureButton.h
        src/ui/components/element/ScrollFrame.cpp
        src/ui/components/element/ScrollFrame.h
        src/editor/ui/menus/editor/components/viewport/Viewport.cpp
        src/editor/ui/menus/editor/components/viewport/Viewport.h
        src/editor/ui/menus/editor/components/viewport/ViewportItem.cpp
        src/editor/ui/menus/editor/components/viewport/ViewportItem.h
        src/ui/components/element/CheckBox.cpp
        src/ui/components/element/CheckBox.h
        src/ui/components/element/Scrollable.cpp
        src/ui/components/element/Scrollable.h
        src/ui/structures/visual_texture/files/HitboxFile.cpp
        src/ui/structures/visual_texture/files/HitboxFile.h
        src/editor/ui/menus/editor/components/texture_button/TextureStateIndicator.cpp
        src/editor/ui/menus/editor/components/texture_button/TextureStateIndicator.h
)

target_include_directories(DrawSomething PRIVATE src)
target_link_libraries(${PROJECT_NAME} SDL3 SDL3_image SDL3_mixer SDL3_ttf ws2_32 libcurl.a curl)
target_link_options(${PROJECT_NAME} PRIVATE "-static")

target_include_directories(DrawSomethingEditor PRIVATE src)
target_link_libraries(DrawSomethingEditor SDL3 SDL3_image SDL3_mixer SDL3_ttf)
target_link_options(DrawSomethingEditor PRIVATE "-static")

file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

function(copy_files SOURCE_DIR DEST_DIR)
    file(GLOB items "${SOURCE_DIR}/*")
    foreach (item ${items})
        if (IS_DIRECTORY ${item})
            get_filename_component(dirname ${item} NAME)
            file(MAKE_DIRECTORY ${DEST_DIR}/${dirname})
            copy_files(${item} ${DEST_DIR}/${dirname})
        else ()
            get_filename_component(filename ${item} NAME)
            configure_file(${item} ${DEST_DIR}/${filename} COPYONLY)
        endif ()
    endforeach ()
endfunction()

copy_files(${CMAKE_CURRENT_SOURCE_DIR}/bin ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

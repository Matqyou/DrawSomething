#define SDL_MAIN_HANDLED
#include "../shared/Protocol.h"

#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "ui/menus/intermission/IntermissionMenu.h"
#include "ui/components/event/EventContext.h"
#include "ui/menus/picking/PickingMenu.h"
#include "ui/menus/ingame/IngameMenu.h"
#include "ui/menus/auth/AuthMenu.h"
#include "ui/menus/main/MainMenu.h"
#include "ui/particles/Particles.h"
#include "network/NetworkClient.h"
#include "ui/cursors/Cursors.h"
#include "core/Application.h"
#include "ui/CommonUI.h"
#include "game/account/Account.h"
#include "game/GameData.h"

#include <windows.h>

void exit_application()
{
	Packet logout("/logout", "POST");
	logout.Send(true);

	Application::destroy();
	std::wcout << L"Graceful exit\n";
	exit(0);
}

bool gConsoleVisible = true;

void ToggleConsole() {
	HWND console = GetConsoleWindow();
	if (console) {
		gConsoleVisible = !gConsoleVisible;
		ShowWindow(console, gConsoleVisible ? SW_SHOW : SW_HIDE);
	}
}

void render_debugging(FullscreenMenu *current_menu, bool render_dragging, Vec2i& render_drag_from)
{
	auto application = Application::Get();
	auto drawing = Assets::Get()->GetDrawing();

	current_menu->RenderDebug();
	float mouse_x, mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);

	if (render_dragging)
	{
		int debug_width = (int)mouse_x - render_drag_from.x;
		int debug_height = (int)mouse_y - render_drag_from.y;
		SDL_FRect debug_rect = {
			(float)render_drag_from.x,
			(float)render_drag_from.y,
			(float)debug_width,
			(float)debug_height,
		};
		drawing->SetColor(255, 0, 255, 100);
		drawing->FillRect(debug_rect);

		drawing->SetColor(255, 0, 255, 255);
		drawing->DrawLine(Vec2f((float)render_drag_from.x, 0),
						  Vec2f((float)render_drag_from.x, (float)application->GetHeight()));
		drawing->DrawLine(Vec2f(0, (float)render_drag_from.y),
						  Vec2f((float)application->GetWidth(), (float)render_drag_from.y));

		auto debug_size = Strings::FString("%i, %i", abs(debug_width), abs(debug_height));
		Texture *debug_render = Assets::Get()->RenderTextBlended(CommonUI::sFontSmaller.GetFont()->TTFFont(),
																 debug_size,
																 { 255, 255, 255, 255 });
		Vec2f debug_render_size = debug_render->GetOriginalSize();
		SDL_FRect text_rect = {
			(float)render_drag_from.x + ((float)debug_width - debug_render_size.x) / 2.0f,
			(float)render_drag_from.y + ((float)debug_height - debug_render_size.y) / 2.0f,
			debug_render_size.x,
			debug_render_size.y,
		};
		drawing->RenderTexture(debug_render->SDLTexture(), nullptr, text_rect);
		delete debug_render;
	}

	drawing->SetColor(255, 0, 255, 255);
	drawing->DrawLine(Vec2f(mouse_x, 0), Vec2f(mouse_x, (float)application->GetHeight()));;
	drawing->DrawLine(Vec2f(0, mouse_y), Vec2f((float)application->GetWidth(), mouse_y));

	auto coordinates = Strings::FString("%i, %i", (int)mouse_x, (int)mouse_y);
	Texture *debug_render = Assets::Get()->RenderTextBlended(CommonUI::sFontSmaller.GetFont()->TTFFont(),
															 coordinates,
															 { 255, 255, 255, 255 });
	Vec2f debug_render_size = debug_render->GetOriginalSize();
	SDL_FRect text_rect = {
		mouse_x - debug_render_size.x,
		mouse_y - debug_render_size.y,
		debug_render_size.x,
		debug_render_size.y,
	};
	drawing->RenderTexture(debug_render->SDLTexture(), nullptr, text_rect);
	delete debug_render;
}

void render_fps_counter(Uint64& last_fps_update,
						Texture **fps_texture,
						SDL_FRect& fps_texture_rect,
						long long frame_duration,
						long long total_frame_duration)
{
	auto assets = Assets::Get();
	auto drawing = assets->GetDrawing();

	if (last_fps_update - SDL_GetTicks() >= 1000)
	{
		last_fps_update = SDL_GetTicks();
		long long fps = (long long)1e9 / frame_duration;
		long long real_fps = (long long)1e9 / total_frame_duration;
		delete *fps_texture;
		*fps_texture = assets->RenderTextSolid(CommonUI::sFontSmaller.GetFont()->TTFFont(),
											   Strings::FString("%lld/%lld", fps, real_fps),
											   { 0, 255, 0, 255 });
		Vec2f fps_render_size = (*fps_texture)->GetOriginalSize();
		fps_texture_rect = { 0, 0, fps_render_size.x, fps_render_size.y };
	}

	if (fps_texture != nullptr)
		drawing->RenderTexture((*fps_texture)->SDLTexture(), nullptr, fps_texture_rect);
}

int main()
{
#ifdef NDEBUG
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
#endif

	// Set the console output to UTF-8
	SetConsoleOutputCP(CP_UTF8);

	// Enable UTF-8 processing in the console
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(hOut, &mode);
	SetConsoleMode(hOut, mode | ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);

	Application::init(new App(APPLICATION_NAME,
							  APPLICATION_VERSION,
							  "com.matq.draw_something",
							  Vec2i(1024, 720),
							  75.0, 10.0));
	auto application = Application::Get();
	auto assets = Assets::Get();
	auto drawing = application->GetDrawing();
	auto clock = application->GetClock();

	SDL_Event sdl_event;
	// Loading loop
	do
	{
		// Events
		while (SDL_PollEvent(&sdl_event))
		{
			application->Event(sdl_event);
			switch (sdl_event.type)
			{
				case SDL_EVENT_QUIT:
				{
					exit_application();
					break;
				}
			}
		}

		// Ticking
		assets->ThreadedLoading();

		// Drawing
		if (clock->TimePassed())
		{
			drawing->SetRenderTarget(nullptr);
			drawing->SetColor(0, 0, 0, 255);
			drawing->Clear();

			SDL_FRect progress_rect = {
				((float)application->GetWidth() - 800.0f) / 2.0f,
				((float)application->GetHeight() - 50.0f) / 2.0f,
				800.0f * ((float)AssetsClass::GetWorkDone() / (float)AssetsClass::GetTotalWork()),
				50.0f
			};
			drawing->SetColor(104, 195, 235, 255);
			drawing->FillRect(progress_rect);

			progress_rect.w = 800.0f;
			progress_rect.h = 50.0f;
			drawing->DrawRect(progress_rect);

			drawing->UpdateWindow();
		}
	} while (AssetsClass::IsLoading());

	SDL_StartTextInput(Application::Get()->GetWindow());

	Centralized.InitUI();

	Centralized.picking_menu->SetPickCallback([](Picking::WordOption *word_option)
											  {
												  const char *Difficulties[] = { "EASY", "MEDIUM", "HARD" };

												  json pick_data;
												  pick_data["game_id"] = Centralized.GetCurrentGame()->game_id;
												  pick_data["picked_word"] = word_option->GetWord();
												  pick_data["picked_difficulty"] = Difficulties[word_option->GetDifficulty()];
												  auto packet = (new Packet("/pick_word", "POST", &pick_data));
												  packet->SetErroredCallback([packet]()
																			 {
																				 delete packet;
																			 });
												  packet->SetResponseCallback([packet](const NetworkResponse& server_response)
																			  {
																				  std::string code = server_response.GetCode();
																				  std::string message = server_response.GetMsg();

																				  if (code == "pick_word_success")
																				  {
																					  if (server_response.response_json.contains("game") && server_response.response_json["game"].is_object())
																					  {
																						  json game_data = server_response.response_json["game"];
																						  int game_id = game_data.value("game_id", -1);
																						  Centralized.GetGame(game_id)->ParseFromJson(game_data, true);
																					  }

																					  Centralized.current_menu = (FullscreenMenu *)(Centralized.ingame_menu);
																					  Centralized.ingame_menu->PrepareDraw();
																					  Centralized.ingame_menu->RefreshMenu();
																				  }
																				  else if (code == "pick_word_fail")
																				  {
																					  dbg_msg("&d[SERVER Response]&r Failed to pick word: %s\n", message.c_str());
																				  }

																				  delete packet;
																			  });
												  packet->Send();
											  });
	Centralized.ingame_menu->SetDoneDrawingCallback([]()
													{
														auto replay = Centralized.ingame_menu->GetCanvas()->SerializeReplay();
														dbg_msg("Length of replay in characters: %d\n", replay.length());

														json recording_data;
														recording_data["game_id"] = Centralized.GetCurrentGame()->game_id;
														recording_data["recording_data"] = replay;
														auto packet = (new Packet("/finished_drawing", "POST", &recording_data));
														packet->SetErroredCallback([packet]()
																				   {
																					   delete packet;
																				   });
														packet->SetResponseCallback([packet](const NetworkResponse& server_response)
																					{
																						std::string code = server_response.GetCode();
																						std::string message = server_response.GetMsg();

																						if (code == "finish_drawing_success")
																						{
																							if (server_response.response_json.contains("game") && server_response.response_json["game"].is_object())
																							{
																								json game_data = server_response.response_json["game"];
																								int game_id = game_data.value("game_id", -1);
																								Centralized.GetGame(game_id)->ParseFromJson(game_data, true);
																								Centralized.SortGames();
																							}

																							Centralized.current_menu = (FullscreenMenu *)Centralized.main_menu;
																							Centralized.main_menu->Games()->RefreshData();
																							Centralized.main_menu->Profile()->RefreshData();
																							Centralized.main_menu->RefreshMenu();

																							dbg_msg("&d[SERVER Response]&r Finished drawing: %s\n", message.c_str());
																						}
																						else if (code == "finish_drawing_fail")
																						{
																							dbg_msg("&d[SERVER Response]&r Failed to upload drawing: %s\n", message.c_str());
																						}

																						delete packet;
																					});
														packet->Send();
													});
	Centralized.intermission_menu->SetEndAnimationCallback([]()
														   {
															   Centralized.current_menu = (FullscreenMenu *)Centralized.main_menu;
															   Centralized.main_menu->Games()->RefreshData();
															   Centralized.main_menu->Profile()->RefreshData();
															   Centralized.main_menu->RefreshMenu();
														   });

	Packet ping_packet("/ping", "GET", nullptr);
	ping_packet.SetResponseCallback([](const NetworkResponse& server_response)
									{
										dbg_msg("&d[SERVER Response]&r Received pong from server.\n");
									})
		->SetErroredCallback([]()
							 {
								 dbg_msg("Could not ping :((\n");
							 });
	ping_packet.Send();

	Centralized.auth_menu->SetLoginDetails("admin", "aaaa");

	Vec2i render_drag_from = Vec2i(0, 0);
	bool render_dragging = false;
	bool render_debug = false;
	long long frame_duration = 1;
	long long total_frame_duration = 1;
	bool update_frame_duration = true;
	Uint64 last_fps_update = 0;
	Texture *fps_texture = nullptr;
	SDL_FRect fps_texture_rect;

	EventContext event_context;
	NetworkResponse server_response;
	drawing->SetDrawBlendMode(SDL_BLENDMODE_BLEND);
	while (true)
	{
		event_context.ResetContext();
		while (SDL_PollEvent(&sdl_event))
		{
			event_context.ResetRapidContext();
			application->Event(sdl_event);
			Centralized.current_menu->HandleEvent(sdl_event, event_context);

			switch (sdl_event.type)
			{
				case SDL_EVENT_QUIT:
				{
					exit_application();
					break;
				}
				case SDL_EVENT_MOUSE_MOTION:
				{
					event_context.had_mouse_motion = true;
					break;
				}
				case SDL_EVENT_KEY_DOWN:
				{

					if (sdl_event.key.scancode == SDL_SCANCODE_F1)
						ToggleConsole();

					if (sdl_event.key.scancode == SDL_SCANCODE_9) Centralized.current_menu->DebugPrint();
					else if (sdl_event.key.scancode == SDL_SCANCODE_0)
						Assets::Get()->SaveTextureToDisk(Centralized.ingame_menu->canvas->GetCanvasTexture(), "canvas_export.png");
					else if (sdl_event.key.scancode == SDL_SCANCODE_GRAVE) render_debug = !render_debug;
					else if (sdl_event.key.scancode == SDL_SCANCODE_4)
					{
						auto packet = (new Packet("/logout", "POST"));
						packet->SetErroredCallback([packet]()
												   { delete packet; });
						packet->SetResponseCallback([packet](const NetworkResponse& server_response)
													{ delete packet; });
						packet->Send();

						Centralized.current_menu = (FullscreenMenu *)Centralized.auth_menu;
						Centralized.auth_menu->RefreshMenu();
					}
					else if (sdl_event.key.scancode == SDL_SCANCODE_5)
					{
						Centralized.current_menu = (FullscreenMenu *)Centralized.main_menu;
						Centralized.main_menu->RefreshMenu();

						// Create new request for fetching games for the user
						auto packet = (new Packet("/get_games", "GET"));
						packet->SetErroredCallback([packet]()
												   { delete packet; });
						// Insert all the received data into the ui elements
						packet->SetResponseCallback([packet](const NetworkResponse& server_response)
													{
														std::string message = server_response.GetMsg();

														// Update incoming data about the current user
														auto& account = Centralized.GetAccount();
														if (server_response.response_json.contains("user") && server_response.response_json["user"].is_object())
														{
															json user_data = server_response.response_json["user"];
															account.ParseFromJson(user_data);
														}

														// Update incoming daa about all the games the user is currently in
														if (server_response.response_json.contains("games") && server_response.response_json["games"].is_array())
														{
															Centralized.ClearGames();
															for (const json& game_data : server_response.response_json["games"])
																// Add the graphical elements
																if (game_data.is_object())
																	Centralized.AddGame((new GameInfo())->ParseFromJson(game_data, false));
															Centralized.SortGames();
														}

														// Update the ui layout with new information and hitboxes
														Centralized.main_menu->Header()->RefreshData();
														Centralized.main_menu->Games()->RefreshData();
														Centralized.main_menu->RefreshMenu();

														dbg_msg("&d[SERVER Response]&r Updated games list: %s\n", message.c_str());
														delete packet;
													});
						packet->Send();
					}

					break;
				}
				case SDL_EVENT_WINDOW_MOUSE_LEAVE:
				{
					auto window_flags = SDL_GetWindowFlags(application->GetWindow());
					if ((window_flags & SDL_WINDOW_INPUT_FOCUS) == 0)
						clock->SetIdleFramerate();

					break;
				}
				case SDL_EVENT_WINDOW_MOUSE_ENTER:
				{
					auto window_flags = SDL_GetWindowFlags(application->GetWindow());
					if ((window_flags & SDL_WINDOW_INPUT_FOCUS) == 0)
						clock->ResetFramerate();

					break;
				}
				case SDL_EVENT_WINDOW_FOCUS_LOST:
				{
					clock->SetIdleFramerate();
					break;
				}
				case SDL_EVENT_WINDOW_FOCUS_GAINED:
				{
					clock->ResetFramerate();
					break;
				}
				case SDL_EVENT_MOUSE_BUTTON_DOWN:
				{
					if (sdl_event.button.button == SDL_BUTTON_RIGHT)
					{
						float mouse_x, mouse_y;
						SDL_GetMouseState(&mouse_x, &mouse_y);
						render_drag_from = { (int)mouse_x, (int)mouse_y };
						render_dragging = true;
					}
					break;
				}
				case SDL_EVENT_MOUSE_BUTTON_UP:
				{
					if (sdl_event.button.button == SDL_BUTTON_RIGHT)
						render_dragging = false;
					break;
				}
			}

			Element::DestroyElements(); // todo: might be expensive
		}
		if (event_context.had_mouse_motion &&
			event_context.cursor_changed == CursorChange::NO_CHANGE)
			SDL_SetCursor(SDL_GetDefaultCursor());

		// Networking
		NetworkClient::TickResolve();
		NetworkClient::TickErrored();

		if (clock->TimePassed())
		{
			update_frame_duration = true;

			// Post-Event
			Centralized.current_menu->PostEvent();

			// Ticking
			Centralized.current_menu->Tick((double)(total_frame_duration) / 1e9);
			Particles.Tick();

			// Drawing
			drawing->SetRenderTarget(nullptr);

			Centralized.current_menu->Render();

			if (render_debug)
				render_debugging(Centralized.current_menu, render_dragging, render_drag_from);

			Particles.Render();

			render_fps_counter(last_fps_update, &fps_texture, fps_texture_rect, frame_duration, total_frame_duration);

			drawing->UpdateWindow();
		}
		if (update_frame_duration)
		{
			update_frame_duration = false;
			frame_duration = clock->GetTimeElapsedNanoNow();
		}

		total_frame_duration = clock->GetTimeElapsedNanoNow();
	}

	return 0;
}

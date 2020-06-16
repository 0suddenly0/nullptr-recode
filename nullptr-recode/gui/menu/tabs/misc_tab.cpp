#include "../menu.h"
#include "../../../functions/misc/misc.h"
#include "../../../sdk/structures/structures.h"

char* clantag_tabs[] =
{
	"standart",
	"players tags",
	"custom"
};

namespace menu {
	void misc_tab() {
		null_gui::create_columns(2);
		static int testr;
		null_gui::begin_group("misc", vec2(0, 300)); {

			null_gui::color_edit("menu color", &globals::menu_color);
			null_gui::check_box("watermark", &settings::misc::watermark);
			null_gui::check_box("obs bypass", &settings::misc::obs_bypass);
			null_gui::check_box("bhop", &settings::misc::bhop::enable);	
			null_gui::combo("auto strafe type", &settings::misc::bhop::strafe_type, std::vector<std::string>{"none", "legt", "pseudo wasd"});

			null_gui::check_box("edge jump", &settings::misc::edge_jump::enable);
			null_gui::tooltip_items("##edge_jump_tooltip", []() {
				null_gui::check_box("auto duck", &settings::misc::edge_jump::auto_duck);
				});
			null_gui::key_bind("##edge_jump_bind", &settings::misc::edge_jump::bind);

			null_gui::check_box("block bot", &settings::misc::block_bot::enable);
			null_gui::key_bind("##bock_bot_bind", &settings::misc::block_bot::bind);

			null_gui::check_box("fake latency", &settings::misc::fake_latency::enable);
			null_gui::slider_int("amont##fake latency", &settings::misc::fake_latency::amount, 0, 1000);

			null_gui::check_box("ingame radar", &settings::visuals::ingame_radar);
			null_gui::tooltip("shows enemies on game radar");

			null_gui::tooltip_items("view model", []() {
				null_gui::slider_float("override##view_model", &settings::misc::viewmodel::override, 60, 150);
				null_gui::slider_float("viewmodel##view_model", &settings::misc::viewmodel::viewmodel, 42, 120);
				null_gui::slider_float("x##view_model", &settings::misc::viewmodel::fov_x, -30, 30);
				null_gui::slider_float("y##view_model", &settings::misc::viewmodel::fov_y, -30, 30);
				null_gui::slider_float("z##view_model", &settings::misc::viewmodel::fov_z, -30, 30);
				null_gui::slider_float("roll##view_model", &settings::misc::viewmodel::roll, -180, 180);

				null_gui::check_box("save roll side", &settings::misc::viewmodel::save_roll_side);
				null_gui::tooltip("ignore cl_righthand");
				});

			null_gui::check_box("auto accept", &settings::misc::auto_accept);
			null_gui::check_box("money reveal", &settings::misc::reveal_money);
			null_gui::check_box("rank reveal", &settings::misc::reveal_rank);
			null_gui::check_box("moon walk", &settings::misc::moon_walk);
			null_gui::check_box("fast duck", &settings::misc::fast_duck);
			null_gui::check_box("prepare revolver", &settings::misc::prepare_revolver::enable);
			null_gui::key_bind("##prepare_revolver_bind", &settings::misc::prepare_revolver::bind);
			null_gui::check_box("slow gravity", &settings::misc::inverse_gravity::enable_slow);
			null_gui::check_box("inverse gravity", &settings::misc::inverse_gravity::enable);
			null_gui::slider_int("inverse gravity value", &settings::misc::inverse_gravity::value, -10, 10);

			null_gui::multi_combo("disable##misc", std::vector<std::string>{ "smoke", "flash", "zoom", "zoom border" }, std::vector<bool*>{ &settings::misc::disable_smoke, &settings::misc::disable_flash, &settings::misc::disable_zoom, &settings::misc::disable_zoom_border });
			if (settings::misc::disable_flash) {
				null_gui::slider_int("max alpha##disable_flash", &settings::misc::flash_alpha, 0, 255);
			}

			null_gui::check_box("thirdperson", &settings::misc::third_person::enable);
			null_gui::key_bind("##thirdperson_bind", &settings::misc::third_person::bind);
			if (settings::misc::third_person::enable) {
				null_gui::slider_int("dist##thirdperson_dist", &settings::misc::third_person::dist, 25, 350);
			}

			null_gui::end_group();
		}

		null_gui::begin_group("clantag##misc_tab", vec2(0, 212));
		{
			null_gui::horizontal(settings::misc::clantag::clantag_type, clantag_tabs);

			ImGui::Checkbox("enable##clantag", &settings::misc::clantag::enable);

			switch (settings::misc::clantag::clantag_type)
			{
			case 0:
				null_gui::combo("clantag's##clanTag", &settings::misc::clantag::animation_type, std::vector<std::string>{"nullptr", "gaysence", "polakware.net", "c l o w n"});
				break;

			case 1:
				if (sdk::player_resource && sdk::engine_client->is_connected()) {

					null_gui::functional_combo("player tags##clantags.list", "-", []() {
						std::vector<std::string> tags;

						if (null_gui::selectable("none")) misc::clan_tag::user_index = 0;

						for (int i = 1; i < sdk::engine_client->get_max_clients(); ++i) {
							auto* player = c_base_player::get_player_by_index(i);

							if (!player) continue;

							const auto info = player->get_player_info();
							if (info.fakeplayer) continue;

							auto user_tag = std::string((*sdk::player_resource)->clantag()[player->get_index()]);
							auto user_name = std::string(info.szName);
							if (settings::misc::clantag::check_empty) {
								if (user_tag.empty() || std::find(tags.begin(), tags.end(), user_tag) != tags.end()) continue;
							}

							tags.push_back(user_tag);

							if (player->get_index() != sdk::local_player->get_index()) {
								if (null_gui::selectable(utils::snprintf("%s : %s [%d]", user_name.c_str(), user_tag.c_str(), player->get_index()).c_str())) {
									misc::clan_tag::user_index = i;
								}
							}
						}
					});

					null_gui::check_box("checking on empty tag at player", &settings::misc::clantag::check_empty);
				}
				else {
					null_gui::text("connect to server");
				}
				break;
			case 2:
				static std::string local_tag;
				null_gui::text_input("clantag##misc_clantag", &local_tag);
				if (null_gui::button("apply##clan", true)) {
					settings::misc::clantag::clantag = local_tag;
					settings::misc::clantag::clantag_visible = local_tag;
				}

				null_gui::combo("animation", &settings::misc::clantag::custom_type, std::vector<std::string>{"none", "type 1", "type 2"});
				if (settings::misc::clantag::clantag_type != 0) {
					null_gui::slider_float("speed", &settings::misc::clantag::speed, 0.1f, 2.f);
				}
				break;
			}
		}
		null_gui::end_group();

		null_gui::begin_group("windows", vec2(0, 100)); {

			null_gui::check_box("bind window", &settings::misc::bind_window);

			null_gui::end_group();
		}
	}
}
#include "settings.h"

std::map<int, std::string> mode_names = {
	{0,"matchmaking"},
	{1,"wingman"},
	{2,"dangerzone"}
};

namespace settings {
	namespace legitbot {
		std::map<int, legitbot_settings_t> legitbot_items = { };
		std::map<int, legitbot_settings_t> legitbot_items_groups = { };
	}
	namespace changers {
		namespace profile {
			std::map<int, profile_settings_t> profile_items = { };
		}

		namespace skin {
			std::map<int, statrack_setting_t> statrack_items = { };
			std::map<int, item_setting_t> m_items = { };
			std::map<std::string, std::string> m_icon_overrides = { };
		}
	}
}
#include "glow.h"

namespace glow {
    void init() { 
        for (auto i = 0; i < sdk::glow_obj_manager->m_GlowObjectDefinitions.count(); i++) {
            glow_object_definition& glow_object = sdk::glow_obj_manager->m_GlowObjectDefinitions[i];
            auto entity = (c_base_player*)(glow_object.m_pEntity);

            if (glow_object.is_unused()  || !entity || entity->is_dormant())
                continue;

            auto class_id = entity->get_client_class()->class_id;

            color color(0, 0, 0, 0);
            int type = 0;

            switch (class_id) {
            case class_id::c_cs_player: {
                bool is_visible = sdk::local_player->can_see_player(entity, hitbox_chest);

                glow_settings_t cur_settings;

                if (entity->team_num() != sdk::local_player->team_num()) {
                    cur_settings = settings::visuals::glow::glow_items[esp_types::enemies];
                } else if (entity == sdk::local_player) {
                    cur_settings = settings::visuals::glow::glow_items[esp_types::local_player];
                } else if (entity->team_num() != sdk::local_player->team_num()) {
                    cur_settings = settings::visuals::glow::glow_items[esp_types::teammates];
                }

                if (!cur_settings.enable)  continue;

                if (is_visible) {
                    color = cur_settings.visible;
                } else if (!is_visible && !cur_settings.visible_only) {
                    color = cur_settings.in_visible;
                }

                type = cur_settings.type;

            } break;
            }

            glow_object.m_flRed = color.r<float>();
            glow_object.m_flGreen = color.g<float>();
            glow_object.m_flBlue = color.b<float>();
            glow_object.m_flAlpha = color.a<float>();
            glow_object.m_nGlowStyle = type;
            glow_object.m_bRenderWhenOccluded = true;
            glow_object.m_bRenderWhenUnoccluded = false;
        }
    }

    void shutdown() {
        for (auto i = 0; i < sdk::glow_obj_manager->m_GlowObjectDefinitions.count(); i++) {
            auto& glow_object = sdk::glow_obj_manager->m_GlowObjectDefinitions[i];
            auto entity = (c_base_player*)(glow_object.m_pEntity);

            if (glow_object.is_unused()) continue;
            if (!entity || entity->is_dormant()) continue;

            glow_object.m_flAlpha = 0.0f;
        }
    }
}

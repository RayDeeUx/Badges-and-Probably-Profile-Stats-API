#pragma once

#include <Geode/cocos/base_nodes/CCNode.h>
#include <Geode/loader/Event.hpp>

enum class ModStatus {
    NONE = 0,
    REGULAR = 1,
    ELDER = 2,
    LEADERBOARD = 3
};

struct UserInfo {
    std::string userName;
    int userID;
    int accountID;
    ModStatus modStatus;
};

struct Badge {
    std::string badgeID;
    geode::Ref<cocos2d::CCNode> targetNode;
};

using BadgeCallback = std::function<cocos2d::CCNodeRGBA*()>;
using ProfileCallback = std::function<void(const Badge& badge, const UserInfo& score)>;

struct BadgeInfo {
    std::string id;
	std::string name;
	std::string description;
    BadgeCallback createBadge;
    ProfileCallback onProfile;
};

namespace BadgesAPI {
    struct RegisterBadgeEvent final : geode::Event {
        RegisterBadgeEvent() {}
        using Fn = void(const std::string& id, const std::string& name, const std::string& description, BadgeCallback&& createBadge, ProfileCallback&& onProfile);
        Fn* fn = nullptr;
    };

    struct ShowBadgeEvent final : geode::Event {
        ShowBadgeEvent() {}
        using Fn = void(const Badge& badge);
        Fn* fn = nullptr;
    };

    inline bool isLoaded() {
        return geode::Loader::get()->getLoadedMod("alphalaneous.badges_api_reimagined") != nullptr;
    }

    template <typename F>
    void waitForBadges(F&& callback) {
        if (isLoaded()) {
            callback();
        } else {
            auto mod = geode::Loader::get()->getInstalledMod("alphalaneous.badges_api_reimagined");
            if (!mod) return;

            new geode::EventListener(
                [callback = std::forward<F>(callback)](geode::ModStateEvent*) {
                    callback();
                },
                geode::ModStateFilter(mod, geode::ModEventType::Loaded)
            );
        }
    }

    template <typename F, typename G>
    inline void registerBadge(const std::string& id, const std::string& name, const std::string& description, F&& createBadge, G&& onProfile) {
        waitForBadges([=] {
            static auto fn = ([] {
                RegisterBadgeEvent event;
                event.post();
                return event.fn;
            })();
            if (fn) fn(id, name, description, createBadge, onProfile);
        });
    }

    inline void showBadge(const Badge& badge) {
        static auto fn = ([] {
            ShowBadgeEvent event;
            event.post();
            return event.fn;
        })();
        if (fn) fn(badge);
    }
}
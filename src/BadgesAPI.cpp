#include "BadgesAPI.hpp"
#include <Geode/Geode.hpp>
#include "Badges.hpp"

using namespace geode::prelude;

$execute {
    new EventListener<EventFilter<BadgesAPI::RegisterBadgeEvent>>(+[](BadgesAPI::RegisterBadgeEvent* event) {
        event->fn = +[](const std::string& id, const std::string& name, const std::string& description, BadgeCallback&& createBadge, ProfileCallback&& onProfile) {
            Badges::get()->registerBadge(id, name, description, std::move(createBadge), std::move(onProfile));
        };
        return ListenerResult::Stop;
    });

    new EventListener<EventFilter<BadgesAPI::ShowBadgeEvent>>(+[](BadgesAPI::ShowBadgeEvent* event) {
        event->fn = +[](const Badge& badge) {
            Badges::get()->showBadge(badge);
        };
        return ListenerResult::Stop;
    });
}
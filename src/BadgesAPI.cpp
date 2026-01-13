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

    new EventListener<EventFilter<BadgesAPI::UnregisterBadgeEvent>>(+[](BadgesAPI::UnregisterBadgeEvent* event) {
        event->fn = +[](const std::string& id) {
            Badges::get()->unregisterBadge(id);
        };
        return ListenerResult::Stop;
    });

    new EventListener<EventFilter<BadgesAPI::SetBadgeNameEvent>>(+[](BadgesAPI::SetBadgeNameEvent* event) {
        event->fn = +[](const std::string& id, const std::string& name) {
            Badges::get()->setName(id, name);
        };
        return ListenerResult::Stop;
    });

    new EventListener<EventFilter<BadgesAPI::GetBadgeNameEvent>>(+[](BadgesAPI::GetBadgeNameEvent* event) {
        event->fn = +[](const std::string& id) {
            return Badges::get()->getName(id);
        };
        return ListenerResult::Stop;
    });

    new EventListener<EventFilter<BadgesAPI::SetBadgeDescriptionEvent>>(+[](BadgesAPI::SetBadgeDescriptionEvent* event) {
        event->fn = +[](const std::string& id, const std::string& description) {
            Badges::get()->setDescription(id, description);
        };
        return ListenerResult::Stop;
    });

    new EventListener<EventFilter<BadgesAPI::GetBadgeDescriptionEvent>>(+[](BadgesAPI::GetBadgeDescriptionEvent* event) {
        event->fn = +[](const std::string& id) {
            return Badges::get()->getDescription(id);
        };
        return ListenerResult::Stop;
    });

    new EventListener<EventFilter<BadgesAPI::SetBadgeCreateCallbackEvent>>(+[](BadgesAPI::SetBadgeCreateCallbackEvent* event) {
        event->fn = +[](const std::string& id, BadgeCallback&& createBadge) {
            Badges::get()->setCreateBadgeCallback(id, std::move(createBadge));
        };
        return ListenerResult::Stop;
    });

    new EventListener<EventFilter<BadgesAPI::SetBadgeProfileCallbackEvent>>(+[](BadgesAPI::SetBadgeProfileCallbackEvent* event) {
        event->fn = +[](const std::string& id, ProfileCallback&& onProfile) {
            Badges::get()->setProfileCallback(id, std::move(onProfile));
        };
        return ListenerResult::Stop;
    });

    new EventListener<EventFilter<BadgesAPI::SetBadgeShownInCommentsEvent>>(+[](BadgesAPI::SetBadgeShownInCommentsEvent* event) {
        event->fn = +[](const std::string& id, const bool shouldShow) {
            Badges::get()->setShouldShowInComments(id, shouldShow);
        };
        return ListenerResult::Stop;
    });

    new EventListener<EventFilter<BadgesAPI::GetBadgeShownInCommentsEvent>>(+[](BadgesAPI::GetBadgeShownInCommentsEvent* event) {
        event->fn = +[](const std::string& id) {
            return Badges::get()->getShouldShowInComments(id);
        };
        return ListenerResult::Stop;
    });

    new EventListener<EventFilter<BadgesAPI::SetBadgeShownInProfilesEvent>>(+[](BadgesAPI::SetBadgeShownInProfilesEvent* event) {
        event->fn = +[](const std::string& id, const bool shouldShow) {
            Badges::get()->setShouldShowInProfiles(id, shouldShow);
        };
        return ListenerResult::Stop;
    });

    new EventListener<EventFilter<BadgesAPI::GetBadgeShownInProfilesEvent>>(+[](BadgesAPI::GetBadgeShownInProfilesEvent* event) {
        event->fn = +[](const std::string& id) {
            return Badges::get()->getShouldShowInProfiles(id);
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
#include "Badges.hpp"
#include "BadgesCommentCell.hpp"
#include "BadgesProfilePage.hpp"

#include <Geode/Geode.hpp>
#include <Geode/binding/GJUserScore.hpp>

using namespace geode::prelude;

Badges* Badges::get() {
    static Badges instance;
    return &instance;
}

void Badges::showBadge(const Badge& badge) {
    auto badgeIter = m_badges.find(badge.badgeID);
    if (badgeIter == m_badges.end()) return;

    if (auto cell = static_cast<BadgesCommentCell*>(typeinfo_cast<CommentCell*>(badge.targetNode.data()))) {
        cell->addBadge(badgeIter->second);
    }
    else if (auto profile = static_cast<BadgesProfilePage*>(typeinfo_cast<ProfilePage*>(badge.targetNode.data()))) {
        profile->addBadge(badgeIter->second);
    }
}

void Badges::registerBadge(const std::string& id, const std::string& name, const std::string& description, BadgeCallback&& createBadge, ProfileCallback&& onProfile) {
    m_badges.emplace(id, BadgeInfo{id, name, description, std::move(createBadge), std::move(onProfile)});
}

void Badges::onProfile(CCNode* node, const UserInfo& info) {
    for (const auto& [k, v] : m_badges) {
        if (v.onProfile) {
            v.onProfile({k, node}, info);
        }
    }
}
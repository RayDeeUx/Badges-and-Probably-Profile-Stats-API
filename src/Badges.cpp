#include "Badges.hpp"
#include "BadgesAPI.hpp"
#include "BadgesCommentCell.hpp"
#include "BadgesProfilePage.hpp"

#include <Geode/Geode.hpp>

using namespace geode::prelude;

Badges* Badges::get() {
    static Badges instance;
    return &instance;
}

void Badges::showBadge(const Badge& badge) {
    auto badgeIter = m_badges.find(badge.badgeID);
    if (badgeIter == m_badges.end()) return;

    auto badgeInfo = badgeIter->second;

    if (auto cell = static_cast<BadgesCommentCell*>(typeinfo_cast<CommentCell*>(badge.targetNode.data())); cell && badgeInfo.showInComments) {
        cell->addBadge(badgeInfo);
    }
    else if (auto profile = static_cast<BadgesProfilePage*>(typeinfo_cast<ProfilePage*>(badge.targetNode.data())); profile && badgeInfo.showInProfiles) {
        profile->addBadge(badgeInfo);
    }
}

void Badges::setName(const std::string& id, const std::string& name) {
    auto badgeIter = m_badges.find(id);
    if (badgeIter == m_badges.end()) return;
    badgeIter->second.name = name;
}

std::string_view Badges::getName(const std::string& id) {
    auto badgeIter = m_badges.find(id);
    if (badgeIter == m_badges.end()) return "";
    return badgeIter->second.name;
}

void Badges::setDescription(const std::string& id, const std::string& description) {
    auto badgeIter = m_badges.find(id);
    if (badgeIter == m_badges.end()) return;
    badgeIter->second.description = description;
}

std::string_view Badges::getDescription(const std::string& id) {
    auto badgeIter = m_badges.find(id);
    if (badgeIter == m_badges.end()) return "";
    return badgeIter->second.description;
}

void Badges::setCreateBadgeCallback(const std::string& id, BadgeCallback&& createBadge) {
    auto badgeIter = m_badges.find(id);
    if (badgeIter == m_badges.end()) return;
    badgeIter->second.createBadge = createBadge;
}

void Badges::setProfileCallback(const std::string& id, ProfileCallback&& onProfile) {
    auto badgeIter = m_badges.find(id);
    if (badgeIter == m_badges.end()) return;
    badgeIter->second.onProfile = onProfile;
}

void Badges::setShouldShowInComments(const std::string& id, const bool shouldShow) {
    auto badgeIter = m_badges.find(id);
    if (badgeIter == m_badges.end()) return;
    badgeIter->second.showInComments = shouldShow;
}

bool Badges::getShouldShowInComments(const std::string& id) {
    auto badgeIter = m_badges.find(id);
    if (badgeIter == m_badges.end()) return false;
    return badgeIter->second.showInComments;
}

void Badges::setShouldShowInProfiles(const std::string& id, const bool shouldShow) {
    auto badgeIter = m_badges.find(id);
    if (badgeIter == m_badges.end()) return;
    badgeIter->second.showInProfiles = shouldShow;
}

bool Badges::getShouldShowInProfiles(const std::string& id) {
    auto badgeIter = m_badges.find(id);
    if (badgeIter == m_badges.end()) return false;
    return badgeIter->second.showInProfiles;
}

void Badges::registerBadge(const std::string& id, const std::string& name, const std::string& description, BadgeCallback&& createBadge, ProfileCallback&& onProfile) {
    m_badges.emplace(id, BadgeInfo{id, name, description, std::move(createBadge), std::move(onProfile), true, true});
}

void Badges::registerBadgeAdvanced(const std::string& id, const std::string& name, const std::string& description, BadgeCallback&& createBadge, ProfileCallback&& onProfile, const bool showInComments, const bool showInProfiles) {
    m_badges.emplace(id, BadgeInfo{id, name, description, std::move(createBadge), std::move(onProfile), showInComments, showInProfiles});
}

void Badges::unregisterBadge(const std::string& id) {
    m_badges.erase(id);
}

void Badges::onProfile(CCNode* node, const UserInfo& info) {
    for (const auto& [k, v] : m_badges) {
        if (v.onProfile) {
            v.onProfile({k, node}, info);
        }
    }
}
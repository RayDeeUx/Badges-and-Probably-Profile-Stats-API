#pragma once

#include <Geode/modify/CommentCell.hpp>
#include <Geode/Geode.hpp>
#include "Badges.hpp"

using namespace geode::prelude;

class $modify(BadgesCommentCell, CommentCell) {

    struct Fields {
        CCPoint m_originalBadgePosition;
        CCNode* m_usernameNode;
        CCNode* m_usernameMenu;
        CCNode* m_percentNode;
        CCNode* m_badgeNode;
        CCMenuItemSpriteExtra* m_badgesButton;
        std::vector<BadgeInfo> m_badges;
        UserInfo m_userInfo;
    };

    void loadFromComment(GJComment* comment);
    void addBadge(const BadgeInfo& info);
    void updateBadges();
    void unregisterTouchesRecursive(CCNode* node);
    void addToBadgeContainer(const BadgeInfo& info);
    void onBadges(CCObject* sender);
};
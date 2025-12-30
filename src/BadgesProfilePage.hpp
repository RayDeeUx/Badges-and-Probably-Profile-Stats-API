#pragma once

#include <Geode/modify/ProfilePage.hpp>
#include <Geode/Geode.hpp>
#include "Badges.hpp"

using namespace geode::prelude;

class $modify(BadgesProfilePage, ProfilePage) {

    struct Fields {
        CCPoint m_originalMenuPosition;
        CCNode* m_usernameMenu;
        CCNode* m_badgeNode;
        CCMenu* m_badgeMenu;
        CCNode* m_infoButton;
        CCSprite* m_separatorLeft;
        CCSprite* m_separatorRight;
        CCMenuItemSpriteExtra* m_badgesButton;
        std::vector<BadgeInfo> m_badges;
        bool m_alterativeDesign;
        UserInfo m_userInfo;
    };
    
    bool init(int accountID, bool ownProfile);
    void loadPageFromUserInfo(GJUserScore* score);
    void addBadge(const BadgeInfo& info);
    void updateBadges();
    void unregisterTouchesRecursive(CCNode* node);
    void addToBadgeContainer(const BadgeInfo& info);
    void onBadges(CCObject* sender);

};
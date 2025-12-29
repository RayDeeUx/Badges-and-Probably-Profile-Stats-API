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
        CCNode* m_infoButton;
		CCMenuItemSpriteExtra* m_badgesButton;
		std::vector<BadgeInfo> m_badges;
	};

    void loadPageFromUserInfo(GJUserScore* score);
	void addBadge(const BadgeInfo& info);
	void updateBadges();
	void addToBadgeContainer(const BadgeInfo& info);
	void onBadges(CCObject* sender);

};
#pragma once
#include "BadgesAPI.hpp"

class BadgesCommentCell;
class BadgesProfilePage;

class Badges {
public:
	static Badges* get();

	void showBadge(const Badge& badge);
    void registerBadge(const std::string& id, const std::string& name, const std::string& description, BadgeCallback&& createBadge, ProfileCallback&& onProfile);
private:
    void onProfile(cocos2d::CCNode* node, const UserInfo& info);
    std::unordered_map<std::string, BadgeInfo> m_badges;

    friend class BadgesCommentCell;
    friend class BadgesProfilePage;
};
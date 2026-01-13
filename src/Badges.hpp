#pragma once
#include "BadgesAPI.hpp"

class BadgesCommentCell;
class BadgesProfilePage;
class BadgesPopup;

class Badges {
public:
    static Badges* get();

    void showBadge(const Badge& badge);

    void setName(const std::string& id, const std::string& name);
    std::string_view getName(const std::string& id);

    void setDescription(const std::string& id, const std::string& description);
    std::string_view getDescription(const std::string& id);

    void setCreateBadgeCallback(const std::string& id, BadgeCallback&& createBadge);
    void setProfileCallback(const std::string& id, ProfileCallback&& onProfile);

    void setShouldShowInComments(const std::string& id, const bool shouldShow);
    bool getShouldShowInComments(const std::string& id);

    void setShouldShowInProfiles(const std::string& id, const bool shouldShow);
    bool getShouldShowInProfiles(const std::string& id);

    void registerBadge(const std::string& id, const std::string& name, const std::string& description, BadgeCallback&& createBadge, ProfileCallback&& onProfile);
    void registerBadgeAdvanced(const std::string& id, const std::string& name, const std::string& description, BadgeCallback&& createBadge, ProfileCallback&& onProfile, const bool showInComments, const bool showInProfiles);

    void unregisterBadge(const std::string& id);

private:
    void onProfile(cocos2d::CCNode* node, const UserInfo& info);
    std::unordered_map<std::string, BadgeInfo> m_badges;

    friend class BadgesCommentCell;
    friend class BadgesProfilePage;
    friend class BadgesPopup;
};
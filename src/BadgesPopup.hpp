#pragma once

#include <Geode/Geode.hpp>
#include "BadgesAPI.hpp"

using namespace geode::prelude;

class BadgesPopup : public Popup<const std::vector<BadgeInfo>&> {
public:
    static BadgesPopup* create(const std::vector<BadgeInfo>& info);
protected:
    bool setup(const std::vector<BadgeInfo>& info) override;
    void goToNextPage(CCObject* sender);
    void goToPrevPage(CCObject* sender);
    void setBadge(int page, float opacity, float xOffset, float scale, bool instant);
    void setNavigationButtonStates();
    void goToPage(bool instant = false);
    void keyDown(cocos2d::enumKeyCodes key) override;

    int m_page = 0;
    CCMenuItemSpriteExtra* m_prevButton;
    CCMenuItemSpriteExtra* m_nextButton;
    CCNode* m_badgesContainer;
    CCLabelBMFont* m_title;
    TextArea* m_description;
    std::map<CCNode*, BadgeInfo> m_badges;
};
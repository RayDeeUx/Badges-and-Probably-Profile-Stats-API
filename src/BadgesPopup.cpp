#include "BadgesPopup.hpp"

BadgesPopup* BadgesPopup::create(const std::vector<BadgeInfo>& info) {
    auto ret = new BadgesPopup();

    auto winSize = CCDirector::get()->getWinSize();

    if (ret->initAnchored(winSize.width, winSize.height, info)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool BadgesPopup::setup(const std::vector<BadgeInfo>& info) {

    setOpacity(0);
    m_bgSprite->removeFromParent();
    m_noElasticity = true;

    auto winSize = CCDirector::get()->getWinSize();

    m_badgesContainer = CCNode::create();
    m_badgesContainer->setID("badges-container");
    m_badgesContainer->setAnchorPoint({0.5f, 0.5f});
    m_badgesContainer->setContentSize({winSize.width / 3.f, 30});
    m_badgesContainer->setScale(3);
    m_badgesContainer->setPosition({winSize.width/2, winSize.height/2 + 30});

    m_mainLayer->setID("main-layer");
    m_mainLayer->addChild(m_badgesContainer);

    m_closeBtn->setPosition({30, winSize.height - 30});
    m_closeBtn->setID("close-button");

    runAction(CCFadeTo::create(0.1f, 220));

    for (const auto& badge : info) {
        if (!badge.createBadge) continue;

        CCNodeRGBA* container = CCNodeRGBA::create();
        container->setID(fmt::format("{}-badge", badge.id).c_str());

        container->setCascadeColorEnabled(true);
        container->setCascadeOpacityEnabled(true);
        container->setAnchorPoint({0.5f, 0.5f});
        container->setOpacity(0);
        
        auto node = badge.createBadge();
        node->setID("badge-node");
        container->addChild(node);

        float scale = m_badgesContainer->getContentHeight() / node->getContentHeight();
        node->setScale(scale);

        container->setContentSize(node->getScaledContentSize());
        node->setPosition(container->getContentSize() / 2.f);

        m_badges[container] = badge;

        m_badgesContainer->addChild(container);
        container->setPositionY(m_badgesContainer->getContentHeight()/2.f);
    }

    m_title = CCLabelBMFont::create(info[0].name.c_str(), "bigFont.fnt");
    m_title->setID("badge-name");

    m_title->setPosition({winSize.width/2, winSize.height - 50});
    m_title->setScale(0.8f);
    m_title->setOpacity(0);

    m_title->runAction(CCFadeTo::create(0.1f, 255));

    auto descScale = 0.75f;

    m_description = TextArea::create(info[0].description, "chatFont.fnt", 1.f, 300.f / descScale, {0.5f, 0.5f}, 20, false);
    m_description->setID("badge-description");
    m_description->setAnchorPoint({0.5f, 1.f});
    m_description->setScale(descScale);
    m_description->setPosition({winSize.width/2, winSize.height/2 - 40});
    m_description->setOpacity(0);

    m_description->runAction(CCFadeTo::create(0.1f, 255));

    m_mainLayer->addChild(m_title);
    m_mainLayer->addChild(m_description);

    auto prevSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");

    m_prevButton = CCMenuItemSpriteExtra::create(prevSpr, this, menu_selector(BadgesPopup::goToPrevPage));
    m_prevButton->setID("prev-button");

    auto nextSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    nextSpr->setFlipX(true);

    m_nextButton = CCMenuItemSpriteExtra::create(nextSpr, this, menu_selector(BadgesPopup::goToNextPage));
    m_nextButton->setID("next-button");

    m_prevButton->setPosition({winSize.width / 2.f - 180, winSize.height / 2.f + 30});
    m_nextButton->setPosition({winSize.width / 2.f + 180, winSize.height / 2.f + 30});

    m_prevButton->setOpacity(0);
    m_nextButton->setOpacity(0);

    m_prevButton->setColor({127, 127, 127});
    m_prevButton->runAction(CCFadeTo::create(0.1f, 127));

    m_prevButton->setEnabled(false);

    if (info.size() == 1) {
        m_nextButton->setEnabled(false);
        m_nextButton->setColor({127, 127, 127});
        m_nextButton->runAction(CCFadeTo::create(0.1f, 127));
    }
    else {
        m_nextButton->runAction(CCFadeTo::create(0.1f, 255));
    }

    m_buttonMenu->setID("main-menu");

    m_buttonMenu->addChild(m_prevButton);
    m_buttonMenu->addChild(m_nextButton);

    m_closeBtn->setOpacity(0);
    m_closeBtn->runAction(CCFadeTo::create(0.1f, 255));

    goToPage(true);

    return true;
}

void BadgesPopup::goToNextPage(CCObject* sender) {
    if (m_page == m_badges.size() - 1) return;
    m_page++;
    goToPage();
}

void BadgesPopup::goToPrevPage(CCObject* sender) {
    if (m_page == 0) return;
    m_page--;
    goToPage();
}

void BadgesPopup::setBadge(int page, float opacity, float xOffset, float scale, bool instant) {
    if (page >= 0 && page <= m_badges.size() - 1) {
        auto node = static_cast<CCNodeRGBA*>(m_badgesContainer->getChildrenExt()[page]);
        node->stopAllActions();
        node->runAction(CCFadeTo::create(0.1f, opacity));

        if (instant) {
            node->setPositionX(m_badgesContainer->getContentWidth()/2.f + xOffset);
            node->setScale(scale);
        }
        else {
            node->runAction(CCMoveTo::create(0.1f, {m_badgesContainer->getContentWidth()/2.f + xOffset, node->getPositionY()}));
            node->runAction(CCScaleTo::create(0.1f, scale));
        }
    }
}

void BadgesPopup::setNavigationButtonStates() {
    m_prevButton->setEnabled(m_page != 0);
    m_nextButton->setEnabled(m_page != m_badges.size() - 1);

    if (m_page == 0) {
        m_prevButton->setColor({127, 127, 127});
        m_prevButton->setOpacity(127);
    }
    else {
        m_prevButton->setColor({255, 255, 255});
        m_prevButton->setOpacity(255);
    }

    if (m_page == m_badges.size() - 1) {
        m_nextButton->setColor({127, 127, 127});
        m_nextButton->setOpacity(127);
    }
    else {
        m_nextButton->setColor({255, 255, 255});
        m_nextButton->setOpacity(255);
    }
}

void BadgesPopup::goToPage(bool instant) {
    setBadge(m_page - 2, 0, -40, 0.25f, instant);
    setBadge(m_page - 1, 127, -30, 0.75f, instant);
    setBadge(m_page, 255, 0, 1.f, instant);
    setBadge(m_page + 1, 127, 30, 0.75f, instant);
    setBadge(m_page + 2, 0, 40, 0.25f, instant);

    m_title->setString(m_badges[m_badgesContainer->getChildrenExt()[m_page]].name.c_str());
    m_description->setString(m_badges[m_badgesContainer->getChildrenExt()[m_page]].description.c_str());

    m_prevButton->setEnabled(m_page != 0);
    m_nextButton->setEnabled(m_page != m_badges.size() - 1);

    if (!instant) {
        setNavigationButtonStates();
    }
}

void BadgesPopup::keyDown(cocos2d::enumKeyCodes key) {
    Popup::keyDown(key);
    if (key == enumKeyCodes::KEY_Left) {
        goToPrevPage(nullptr);
    }
    if (key == enumKeyCodes::KEY_Right) {
        goToNextPage(nullptr);
    }
}
#include "BadgesProfilePage.hpp"
#include "BadgesPopup.hpp"

bool BadgesProfilePage::init(int accountID, bool ownProfile) {
    if (!ProfilePage::init(accountID, ownProfile)) return false;
    auto fields = m_fields.self();
    fields->m_alterativeDesign = Mod::get()->getSettingValue<bool>("alternative-profile-badge-design");

    auto floorLine = static_cast<CCSprite*>(m_mainLayer->getChildByID("floor-line"));
    floorLine->setVisible(false);

    return true;
}

void BadgesProfilePage::loadPageFromUserInfo(GJUserScore* score) {
    auto fields = m_fields.self();
    fields->m_alterativeDesign = Mod::get()->getSettingValue<bool>("alternative-profile-badge-design");

    int originalBadge = score->m_modBadge;

    fields->m_userInfo = {
        score->m_userName,
        score->m_userID,
        score->m_accountID,
        static_cast<ModStatus>(originalBadge)
    };

    score->m_modBadge = 0;
    ProfilePage::loadPageFromUserInfo(score);
    score->m_modBadge = originalBadge;

    fields->m_usernameMenu = m_mainLayer->getChildByID("username-menu");
    auto usernameMenuLayout = static_cast<RowLayout*>(fields->m_usernameMenu->getLayout());
    usernameMenuLayout->ignoreInvisibleChildren(true);

    if (auto badge = fields->m_usernameMenu->getChildByID("mod-badge")) {
        fields->m_usernameMenu->removeChild(badge);
    }

    fields->m_infoButton = m_buttonMenu->getChildByID("info-button");

    if (!fields->m_alterativeDesign) {
        if (!fields->m_badgeNode) {
            fields->m_badgeNode = CCNode::create();
            fields->m_badgeNode->setID("badges-container"_spr);

            fields->m_originalMenuPosition = fields->m_usernameMenu->getPosition();

            auto layout = RowLayout::create();
            layout->setAutoScale(false);
            layout->setAutoGrowAxis(0);
            layout->setAxisReverse(true);

            fields->m_badgeNode->setContentSize({23, 23});
            fields->m_badgeNode->setLayout(layout);

            layout->setGap(-20);

            fields->m_badgesButton = CCMenuItemSpriteExtra::create(fields->m_badgeNode, this, menu_selector(BadgesProfilePage::onBadges));
            fields->m_badgesButton->setID("badges-button"_spr);
            fields->m_badgesButton->setTag(0);

            fields->m_usernameMenu->insertBefore(fields->m_badgesButton, m_usernameLabel);

            updateBadges();
            fields->m_usernameMenu->updateLayout();

            Badges::get()->onProfile(this, fields->m_userInfo);
        }
        else {
            fields->m_badges.clear();
            fields->m_badgeNode->removeAllChildren();
            updateBadges();
            fields->m_usernameMenu->updateLayout();

            Badges::get()->onProfile(this, fields->m_userInfo);
        }
    }
    else {
        if (!fields->m_badgeMenu) {
            fields->m_badgeMenu = CCMenu::create();
            fields->m_badgeMenu->setID("badges-container"_spr);
            fields->m_badgeMenu->ignoreAnchorPointForPosition(false);
            fields->m_badgeMenu->setAnchorPoint({0.5f, 0.5f});
            fields->m_badgeMenu->setContentSize({20, 20});

            auto layout = RowLayout::create();
            layout->setAutoScale(false);
            layout->setAutoGrowAxis(0);
            layout->setGap(3);

            fields->m_badgeMenu->setLayout(layout);

            auto statsMenu = m_mainLayer->getChildByID("stats-menu");
            statsMenu->setPositionY(statsMenu->getPositionY() - 2.5);

            auto floorLine = static_cast<CCSprite*>(m_mainLayer->getChildByID("floor-line"));
            floorLine->setVisible(false);

            auto blendFunc = floorLine->getBlendFunc();

            fields->m_separatorLeft = CCSprite::createWithSpriteFrameName("floorLine_001.png");
            fields->m_separatorLeft->setID("separator-left"_spr);
            auto rect = fields->m_separatorLeft->getTextureRect();
            rect.size.width = 222;
            fields->m_separatorLeft->setTextureRect(rect, fields->m_separatorLeft->isTextureRectRotated(), rect.size);
            fields->m_separatorLeft->setOpacity(100);
            fields->m_separatorLeft->setContentSize({222.f, 1.5f});
            fields->m_separatorLeft->setAnchorPoint({0.f, 0.5});
            fields->m_separatorLeft->setScaleX(0.8f);
            fields->m_separatorLeft->setPosition({floorLine->getPositionX() - fields->m_separatorLeft->getScaledContentWidth(), floorLine->getPositionY() - 2});
            fields->m_separatorLeft->setBlendFunc(blendFunc);

            m_mainLayer->addChild(fields->m_separatorLeft);

            fields->m_separatorRight = CCSprite::createWithSpriteFrameName("floorLine_001.png");
            fields->m_separatorRight->setID("separator-right"_spr);
            fields->m_separatorRight->setOpacity(100);
            fields->m_separatorRight->setTextureRect(rect, fields->m_separatorLeft->isTextureRectRotated(), rect.size);
            fields->m_separatorRight->setFlipX(true);
            fields->m_separatorRight->setContentSize({222.f, 1.5f});
            fields->m_separatorRight->setAnchorPoint({1.f, 0.5});
            fields->m_separatorRight->setScaleX(0.8f);
            fields->m_separatorRight->setPosition({floorLine->getPositionX() + fields->m_separatorRight->getScaledContentWidth(), floorLine->getPositionY() - 2});
            fields->m_separatorRight->setBlendFunc(blendFunc);

            m_mainLayer->addChild(fields->m_separatorRight);

            fields->m_badgeMenu->setPosition({floorLine->getPositionX(), floorLine->getPositionY() - 2});

            m_mainLayer->addChild(fields->m_badgeMenu);

            updateBadges();
            Badges::get()->onProfile(this, fields->m_userInfo);
        }
        else {
            fields->m_badges.clear();
            fields->m_badgeMenu->removeAllChildren();
            updateBadges();
            Badges::get()->onProfile(this, fields->m_userInfo);
        }
    }
}

void BadgesProfilePage::addBadge(const BadgeInfo& info) {
    auto fields = m_fields.self();
    fields->m_badges.push_back(info);
    if (!fields->m_alterativeDesign) {
        if (fields->m_badges.size() <= 3) {
            addToBadgeContainer(info);
        }
    }
    else {
        if (fields->m_badgeMenu->getContentWidth() <= 315) {
            addToBadgeContainer(info);
        }
    }
}

void BadgesProfilePage::updateBadges() {
    auto fields = m_fields.self();
    if (!fields->m_alterativeDesign) {

        fields->m_badgesButton->setVisible(fields->m_badges.size() != 0);
        if (fields->m_badges.size() > 0) {
            fields->m_badgeNode->updateLayout();
            fields->m_badgeNode->setPosition(fields->m_badgeNode->getContentSize()/2);
            fields->m_badgesButton->setContentSize(fields->m_badgeNode->getContentSize());

            m_usernameLabel->limitLabelWidth(140.0f, 0.8f, 0.0f);
            fields->m_usernameMenu->setPositionX(fields->m_originalMenuPosition.x - fields->m_badgesButton->getContentWidth()/2.f);
            fields->m_usernameMenu->updateLayout();

            auto world = fields->m_usernameMenu->convertToWorldSpace(m_usernameLabel->getPosition());
            auto node = m_buttonMenu->convertToNodeSpace(world);

            fields->m_infoButton->setPositionX(node.x + m_usernameLabel->getScaledContentWidth() / 2.f + 8.f);
        }
    }
    else {
        if (fields->m_badges.size() > 0) {
            fields->m_badgeMenu->updateLayout();
            auto rect = fields->m_separatorLeft->getTextureRect();

            auto width = fields->m_badgeMenu->getContentWidth();
            rect.size.width = 222 - ((width / 2.f) / 0.8f) - 5;

            fields->m_separatorLeft->setTextureRect(rect, fields->m_separatorLeft->isTextureRectRotated(), rect.size);
            fields->m_separatorRight->setTextureRect(rect, fields->m_separatorRight->isTextureRectRotated(), rect.size);
        }
    }
}

void BadgesProfilePage::unregisterTouchesRecursive(CCNode* node) {
    if (!node) return;

    if (auto delegate = typeinfo_cast<CCTouchDelegate*>(node)) {
        CCTouchDispatcher::get()->removeDelegate(delegate);
    }

    for (auto child : node->getChildrenExt()) {
        unregisterTouchesRecursive(child);
    }
}

void BadgesProfilePage::addToBadgeContainer(const BadgeInfo& info) {
    auto fields = m_fields.self();
    if (!info.createBadge) return;

    if (!fields->m_alterativeDesign) {
        auto node = info.createBadge();
        
        auto height = node->getContentHeight();
        auto scale = 23 / height;

        node->setScale(scale);
        node->setZOrder(-fields->m_badges.size());
        node->setID(fmt::format("{}-badge", info.id));

        fields->m_badgeNode->addChild(node);
        runAction(CallFuncExt::create([this, node] {
            unregisterTouchesRecursive(node);
        }));

        updateBadges();
    }
    else {
        auto node = info.createBadge();
        
        auto height = node->getContentHeight();
        auto scale = 15 / height;

        node->setScale(scale);
        node->setZOrder(-fields->m_badges.size());
        node->setID(fmt::format("badge-node", info.id));
        
        auto btn = CCMenuItemSpriteExtra::create(node, this, menu_selector(BadgesProfilePage::onBadges));
        btn->setID(fmt::format("{}-badge", info.id));
        btn->setTag(fields->m_badges.size() - 1);

        fields->m_badgeMenu->addChild(btn);
        runAction(CallFuncExt::create([this, node] {
            unregisterTouchesRecursive(node);
        }));
        
        updateBadges();
    }
}

void BadgesProfilePage::onBadges(CCObject* sender) {
    auto fields = m_fields.self();

    BadgesPopup::create(fields->m_userInfo, fields->m_badges, sender->getTag())->show();
}

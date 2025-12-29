#include "BadgesProfilePage.hpp"
#include "BadgesPopup.hpp"

void BadgesProfilePage::loadPageFromUserInfo(GJUserScore* score) {
    auto fields = m_fields.self();
    int originalBadge = score->m_modBadge;

    UserInfo info = {
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

    if (!fields->m_badgeNode) {
        fields->m_badgeNode = CCNode::create();
        fields->m_badgeNode->setID("badges-container"_spr);

        fields->m_originalMenuPosition = fields->m_usernameMenu->getPosition();

        auto layout = RowLayout::create();
        layout->setAutoScale(false);
        layout->setAutoGrowAxis(0);
        layout->setAxisReverse(true);

        fields->m_badgeNode->setContentSize({23, 23});
        layout->setGap(-20);
        
        fields->m_badgeNode->setLayout(layout);

        fields->m_badgesButton = CCMenuItemSpriteExtra::create(fields->m_badgeNode, this, menu_selector(BadgesProfilePage::onBadges));
        fields->m_badgesButton->setID("badges-button"_spr);

        fields->m_usernameMenu->insertBefore(fields->m_badgesButton, m_usernameLabel);

        updateBadges();
        fields->m_usernameMenu->updateLayout();

        Badges::get()->onProfile(this, info);
    }
    else {
        fields->m_badges.clear();
        fields->m_badgeNode->removeAllChildren();
        updateBadges();
        fields->m_usernameMenu->updateLayout();

        Badges::get()->onProfile(this, info);
    }
}

void BadgesProfilePage::addBadge(const BadgeInfo& info) {
    auto fields = m_fields.self();
    fields->m_badges.push_back(info);

    if (fields->m_badges.size() <= 3) {
        addToBadgeContainer(info);
    }
}

void BadgesProfilePage::updateBadges() {
    auto fields = m_fields.self();
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

void BadgesProfilePage::addToBadgeContainer(const BadgeInfo& info) {
    auto fields = m_fields.self();
    if (!info.createBadge) return;
        
    auto node = info.createBadge();
    
    auto height = node->getContentHeight();
    auto containerHeight = fields->m_badgeNode->getContentHeight();
    auto scale = containerHeight / height;

    node->setScale(scale);
    node->setZOrder(-fields->m_badges.size());
    node->setID(fmt::format("{}-badge", info.id));
    fields->m_badgeNode->addChild(node);

    updateBadges();
}

void BadgesProfilePage::onBadges(CCObject* sender) {
    auto fields = m_fields.self();

    BadgesPopup::create(fields->m_badges)->show();
}

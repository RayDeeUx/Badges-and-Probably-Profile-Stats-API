#include "BadgesCommentCell.hpp"
#include "Badges.hpp"
#include "BadgesPopup.hpp"

void BadgesCommentCell::loadFromComment(GJComment* comment) {
    if (comment->m_levelID == 0) return CommentCell::loadFromComment(comment);

    auto fields = m_fields.self();

    int originalBadge = comment->m_modBadge;
    comment->m_modBadge = 1;
    CommentCell::loadFromComment(comment);
    comment->m_modBadge = originalBadge;

    auto mainMenu = m_mainLayer->getChildByID("main-menu");
    auto userMenu = mainMenu->getChildByID("user-menu");
    fields->m_usernameMenu = userMenu->getChildByID("username-menu");

    auto badge = fields->m_usernameMenu->getChildByID("mod-badge");
    fields->m_originalBadgePosition = badge->getPosition();
    
    fields->m_usernameNode = fields->m_usernameMenu->getChildByID("username-button");;
    fields->m_percentNode = fields->m_usernameMenu->getChildByID("percentage-label");;

    fields->m_badgeNode = CCNode::create();
    fields->m_badgeNode->setID("badges-container"_spr);

    auto layout = RowLayout::create();
    layout->setAutoScale(false);
    layout->setAutoGrowAxis(0);
    layout->setAxisReverse(true);

    if (m_compactMode) {
        fields->m_badgeNode->setContentSize({12, 12});
        layout->setGap(-10);
    }
    else {
        fields->m_badgeNode->setContentSize({16, 16});
        layout->setGap(-14);
    }

    fields->m_badgeNode->setLayout(layout);

    fields->m_badgesButton = CCMenuItemSpriteExtra::create(fields->m_badgeNode, this, menu_selector(BadgesCommentCell::onBadges));
    fields->m_badgesButton->setID("badges-button"_spr);

    fields->m_usernameMenu->removeChild(badge);
    fields->m_usernameMenu->insertAfter(fields->m_badgesButton, fields->m_usernameNode);

    UserInfo info = {
        comment->m_userName,
        comment->m_userID,
        comment->m_accountID,
        static_cast<ModStatus>(originalBadge)
    };

    updateBadges();

    Badges::get()->onProfile(this, info);
}

void BadgesCommentCell::addBadge(const BadgeInfo& info) {
    auto fields = m_fields.self();
    fields->m_badges.push_back(info);

    if (fields->m_badges.size() <= 3) {
        addToBadgeContainer(info);
    }
}

void BadgesCommentCell::updateBadges() {
    auto fields = m_fields.self();

    fields->m_badgeNode->updateLayout();
    fields->m_badgesButton->setContentSize({fields->m_badgeNode->getContentWidth(), fields->m_badgeNode->getContentHeight() + 4});
    fields->m_badgeNode->setPosition({fields->m_badgesButton->getContentWidth()/2, fields->m_badgesButton->getContentHeight()/2 + 1});

    fields->m_badgesButton->setPosition({fields->m_usernameNode->boundingBox().getMaxX() + fields->m_badgesButton->getContentWidth() / 2.f, fields->m_originalBadgePosition.y + 2});

    fields->m_badgesButton->setVisible(fields->m_badges.size() != 0);
    fields->m_usernameMenu->updateLayout();
}

void BadgesCommentCell::addToBadgeContainer(const BadgeInfo& info) {
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

void BadgesCommentCell::onBadges(CCObject* sender) {
    auto fields = m_fields.self();

    BadgesPopup::create(fields->m_badges)->show();
}

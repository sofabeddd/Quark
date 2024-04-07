#include <Geode/Geode.hpp>
#include <Geode/modify/CCMotionStreak.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/ProfilePage.hpp>

using namespace geode::prelude;

class $modify(GJGarageLayer) {
    bool init() override {
        if (!GJGarageLayer::init()) return false;

        removeChildByID("tap-more-hint");
        removeChildByID("username-lock");
        removeChildByID("top-left-corner");
        removeChildByID("bottom-left-corner");
        removeChildByID("bottom-right-corner");

        return true;
    }
};

class $modify(RemasteredMenuLayer, MenuLayer) {
    static void onModify(auto& self) {
        (void) self.setHookPriority("MenuLayer::init", -10000);
    }

    bool init() override {
        if (!MenuLayer::init()) return false;

        if (!Mod::get()->getSettingValue<bool>("cleanup-menu")) return true;

        this->removeChildByID("social-media-menu");
        this->removeChildByID("more-games-menu");
        this->removeChildByID("player-username");

        auto bottom_menu = typeinfo_cast<CCMenu*>(getChildByID("bottom-menu"));

        bottom_menu->removeChildByID("newgrounds-button");
        bottom_menu->updateLayout();

        this->getChildByID("profile-menu")->setPositionY(45.f);
        this->getChildByID("right-side-menu")->setPositionY(45.f);

        if (auto deathlink_menu = getChildByID("alphalaneous.deathlink/deathlink-menu")) {
            deathlink_menu->getChildByID("alphalaneous.deathlink/deathlink-button")->setPosition(65.f, 65.f);
        }

        return true;
    }
};

class $modify(PlayLayer) {
    void postUpdate(float dt) {
        PlayLayer::postUpdate(dt);

        if (Mod::get()->getSettingValue<bool>("glow-colored-trails")) {
            m_player1->m_regularTrail->setColor(m_player1->m_glowColor);
            m_player1->m_waveTrail->setColor(m_player1->m_glowColor);

            m_player2->m_regularTrail->setColor(m_player2->m_glowColor);
            m_player2->m_waveTrail->setColor(m_player2->m_glowColor);
        }

        if (Mod::get()->getSettingValue<bool>("same-dual-color")) {
            m_player2->setColor(m_player1->getColor());
            m_player2->setSecondColor(m_player1->getSecondColor());
        }

        if (Mod::get()->getSettingValue<bool>("hide-ship-trail")) {
            if (m_player1->m_isShip) m_player1->m_regularTrail->CCMotionStreak::stopStroke();
            else m_player1->m_regularTrail->CCMotionStreak::resumeStroke();

            if (m_player2->m_isShip) m_player2->m_regularTrail->CCMotionStreak::stopStroke();
            else m_player2->m_regularTrail->CCMotionStreak::resumeStroke();
        }
    }
};

class $modify(ProfilePage) {
    void getUserInfoFinished(GJUserScore *p0) {
        ProfilePage::getUserInfoFinished(p0);

        if (!Mod::get()->getSettingValue<bool>("im-nosy")) return;

        if (!this->getChildByIDRecursive("comment-history-button")) {
            auto comment_history = CCMenuItemSpriteExtra::create(
                    CCSprite::createWithSpriteFrameName("btn_chatHistory_001.png"),
                    this, menu_selector(ProfilePage::onCommentHistory)
            );

            comment_history->setID("comment-history-button");
            comment_history->setPosition({408.f, -135.f});

            this->m_buttonMenu->addChild(comment_history);
        }
    }
};

$execute {
    if (Mod::get()->getSettingValue<bool>("i-did-the-spin-offs")) {
        auto game_manager = GameManager::sharedState();

        int icons[] = {13, 65, 66, 67, 68, 69 /* nice */, 70, 71, 72, 75, 143, 144, 145, 146, 147, 148};
        for (int icon: icons) {
            game_manager->unlockIcon(icon, IconType::Cube);
        }
    }
}
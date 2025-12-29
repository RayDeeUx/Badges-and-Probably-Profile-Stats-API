#include <Geode/Geode.hpp>
#include "BadgesAPI.hpp"

using namespace geode::prelude;

$execute {
	BadgesAPI::registerBadge("moderator"_spr, 
		"Moderator", 
		"A Moderator is identifiable through the Moderator badge in Geometry Dash. This role can be granted only by RobTop himself, meaning that Moderators represent him as well as Geometry Dash directly. Their main task is suggesting good levels to RobTop for a rating. They also have more power when voting for Demon difficulty.", 
		[] {
			return CCSprite::createWithSpriteFrameName("modBadge_01_001.png");
		},
		[] (const Badge& badge, const UserInfo& info) {
			if (info.modStatus == ModStatus::REGULAR) BadgesAPI::showBadge(badge);
		}
	);

	BadgesAPI::registerBadge("elder-moderator"_spr, 
		"Elder Moderator", 
		"An Elder Moderator is identifiable through the Elder Moderator badge in Geometry Dash. Elder Moderators are tasked not only with sending good levels, but also moderating the in-game comment sections, whitelisting users on Newgrounds, unlocking updates for older rated levels, and some other tasks.", 
		[] {
			return CCSprite::createWithSpriteFrameName("modBadge_02_001.png");
		},
		[] (const Badge& badge, const UserInfo& info) {
			if (info.modStatus == ModStatus::ELDER) BadgesAPI::showBadge(badge);
		}
	);

	BadgesAPI::registerBadge("leaderboard-moderator"_spr, 
		"Leaderboard Moderator", 
		"Leaderboard Moderators manage the Leaderboards within the game. They manage the Top 1000 whitelist and can ban players from both the Global and Level Leaderboards. It is worth noting that they are not necessarily a normal Moderator too.", 
		[] {
			return CCSprite::createWithSpriteFrameName("modBadge_03_001.png");
		},
		[] (const Badge& badge, const UserInfo& info) {
			if (info.modStatus == ModStatus::LEADERBOARD) BadgesAPI::showBadge(badge);
		}
	);
}
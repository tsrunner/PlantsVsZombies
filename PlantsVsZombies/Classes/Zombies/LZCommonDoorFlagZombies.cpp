/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.08
 *Email: 2117610943@qq.com
 */

#include "LZCommonDoorFlagZombies.h"

CommonDoorFlagZombies::CommonDoorFlagZombies(Node* node)
{
	_node = node;
	_attackHeadSoundEffectType = 0;
	_attackBodySoundEffectType = 1;
	_bloodVolume = 200;
	_headShieldVolume = 0;
	_bodyShieldVolume = 1200;
	_currentBloodVolume = 200;
	_currentHeadShieldVolume = 0;
	_currentBodyShieldVolume = 1200;
	_speed = 42;
	_currentSpeed = 42;
	_isHaveShield = true;
	_bodyShieldType = ShieldType::IronBodyShield;
	_zombiesType = ZombiesType::CommonDoorFlagZombies;
}

CommonDoorFlagZombies* CommonDoorFlagZombies::create(Node* node)
{
	CommonDoorFlagZombies* commonDoorZombies = new (std::nothrow) CommonDoorFlagZombies(node);
	if (commonDoorZombies && commonDoorZombies->init())
	{
		commonDoorZombies->autorelease();
		return commonDoorZombies;
	}
	CC_SAFE_DELETE(commonDoorZombies);
	return nullptr;
}

void CommonDoorFlagZombies::createZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 3 + 2);

	setZombieScale();

	createZombieShadow();

	setAttachment();
	_zombiesAnimation->setAnimation(0, "Zombies_Door_Walk", true);
}

void CommonDoorFlagZombies::createPreviewZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 2);

	setZombieScale();

	createZombieShadow();

	setAttachment();
	_zombiesAnimation->setAnimation(0, "Zombies_Door_Stand", true);
}

void CommonDoorFlagZombies::playZombieSoundEffect()
{
	Zombies::playZombieSoundEffect(rand() % 2 ? "groan3" : "sukhbir3");
}

void CommonDoorFlagZombies::setAttachment()
{
	_zombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
	_zombiesAnimation->setAttachment("Zombie_screendoor", "Zombie_screendoor1");
	_zombiesAnimation->setAttachment("tt_innerarm_upper", rand() % 4 ? "tt_flag" : "tt_flag1");
	_zombiesAnimation->setAttachment("tt_innerarm_lower", "tt_innerleg_foot3");
	_zombiesAnimation->setAttachment("tt_innerarm_hand", "tt_innerleg_foot3");
}

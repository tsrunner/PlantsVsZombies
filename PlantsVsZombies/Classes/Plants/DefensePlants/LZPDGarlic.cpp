/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#include "LZPDGarlic.h"

#include "Zombies/LZZZombies.h"
#include "Scenes/GameScene/LZSGData.h"
#include "Scenes/SelectPlantsScene/LZSSSpriteLayer.h"
#include "Based/LZBPlayMusic.h"

Garlic::Garlic(Node* node):
    _animationId(0)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 2000;
	_totalHealthPoint = 2000;
	_plantsType = PlantsType::Garlic;

	srand(time(nullptr));
}

Garlic::~Garlic()
{
}

Garlic* Garlic::create(Node* node)
{
	Garlic* garlic = new (std::nothrow) Garlic(node);
	if (garlic && garlic->init())
	{
		garlic->autorelease();
		return garlic;
	}
	CC_SAFE_DELETE(garlic);
	return nullptr;
}
Sprite* Garlic::createPlantImage()
{
	imageInit("Garlic", INIT);
	_plantImage->setScale(1.2f);
	return _plantImage;
}

void Garlic::createPlantAnimation()
{
	_plantAnimation = plantInit("Garlic", "Garlic_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(0.8f);
	_node->addChild(_plantAnimation);

	// Ӱ��
	setPlantShadow(3.0f);

	// �����ɽ�����
	setPlantSoilSplashAnimation(1.0f);
}

void Garlic::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* ��ʬ��ֲ�� */
	}
}

void Garlic::zombieEatPlant(Zombies* zombie)
{
	if (getPlantIsSurvive() && getZombieIsSameLineWithPlant(zombie) && Plants::getZombieIsEncounterPlant(zombie)) /* ֲ���� && ֲ���뽩ʬ��ͬһ�� && ��ʬ����ֲ�� */
	{
		if (zombie->getZombieIsSurvive() && !zombie->getZombieIsEat())
		{
			const string eateffect[3] = { "chomp","chomp2","chompsoft" };
			zombie->setZombieEatPlantNumber(_plantNumber);
			zombie->setZombieStop();
			zombie->setZombieIsEat(true);
			zombie->getZombieAnimation()->setAnimation(0, "Zombies_Eat", true);
			zombie->getZombieAnimation()->setColor(Color3B(181, 230, 29));

			_healthPoint -= 100;

			PlayMusic::playMusic(eateffect[rand() % 3]);
			setPlantHurtBlink();

			zombie->getZombieAnimation()->runAction(Sequence::create(DelayTime::create(1.0f),
				CallFunc::create([=]()
					{
						rand() % 2 == 0 ? PlayMusic::playMusic("squash_hmm") : PlayMusic::playMusic("squash_hmm2");
						auto timeScale = zombie->getZombieAnimation()->getTimeScale();
						zombie->getZombieAnimation()->setTimeScale(0);

						if (zombie->getZombieCurrentBloodVolume() > 0)
						{
							changeZombiePositionY(zombie);

							zombie->getZombieAnimation()->runAction(Sequence::create(DelayTime::create(0.5f),
								CallFunc::create([=]()
									{
										zombie->getZombieAnimation()->setColor(Color3B::WHITE);
										zombie->getZombieAnimation()->setTimeScale(timeScale);
										zombieRecoveryMove(zombie);
									}), nullptr));
						}
					}), nullptr));
		}
	}
}

void Garlic::zombieRecoveryMove(Zombies* zombie)
{
	if (zombie->getZombieEatPlantNumber() == _plantNumber &&   /* ��ʬ�ǳԵĸ�ֲ�� */
		zombie->getZombieIsEat() && zombie->getZombieIsStop()) /* ��ʬ���ڳ�ֲ�� && ��ʬ����ֹͣ�ƶ� */
	{
		zombie->setZombieIsEat(false);
		if (!zombie->getZombieIsPlayDieAnimation()) /* ��ʬû�в����������� */
		{
			zombie->getZombieAnimation()->setMix("Zombies_Eat", Zombies::getZombieAniamtionName(zombie->getZombieType()), 0.5f);
			zombie->getZombieAnimation()->addAnimation(0, Zombies::getZombieAniamtionName(zombie->getZombieType()), true);
			zombie->setZombieCurrentSpeed(zombie->getZombieSpeed());
		}
	}
}

void Garlic::checkPlantHealthPoint()
{
	if (_healthPoint <= static_cast<int>(_totalHealthPoint * 2.f / 3.f) && _animationId == 0)
	{
		_animationId = 1;
		_plantAnimation->setAnimation(0, "Garlic_Damage", true);
	}
	if (_healthPoint <= static_cast<int>(_totalHealthPoint * 1.f / 3.f) && _animationId == 1)
	{
		_animationId = 2;
		_plantAnimation->setAnimation(0, "Garlic_Damage2", true);
	}
	if (_healthPoint <= 0)
	{
		_plantAnimation->setVisible(false);
	}
}

bool Garlic::getPlantIsSurvive() const
{
	return _healthPoint > 0 ? true : false;
}

void Garlic::changeZombiePositionY(Zombies* zombie)
{
	if (zombie->getZombieInRow() == 0)
	{
		zombie->getZombieAnimation()->runAction(MoveBy::create(0.5f, Vec2(-20, 138)));
		zombie->getZombieAnimation()->setLocalZOrder(zombie->getZombieAnimation()->getLocalZOrder() - 20);
		zombie->setZombieInRow(1);
	}
	else if (zombie->getZombieInRow() == 4)
	{
		zombie->getZombieAnimation()->runAction(MoveBy::create(0.5f, Vec2(-20, -138)));
		zombie->getZombieAnimation()->setLocalZOrder(zombie->getZombieAnimation()->getLocalZOrder() + 20);
		zombie->setZombieInRow(3);
	}
	else
	{
		auto number = rand() % 2;
		zombie->getZombieAnimation()->runAction(MoveBy::create(0.5f, Vec2(-20, number ? -138 : 138)));
		zombie->getZombieAnimation()->setLocalZOrder(zombie->getZombieAnimation()->getLocalZOrder() + (number ? 20 : -20));
		zombie->setZombieInRow(number ? zombie->getZombieInRow() - 1 : zombie->getZombieInRow() + 1);
	}
}

SkeletonAnimation* Garlic::showPlantAnimationAndText()
{
	auto& lta = _global->userInformation->getGameText();
	SPSSpriteLayer::plantCardTextScrollView->setInnerContainerSize(Size(lta.find("GARLIC_1")->second->position));

	_isLoop = true;
	_plantAnimation = plantInit("Garlic", "Garlic_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.5f);
	_plantAnimation->setPosition(Vec2(200, 610));

	SPSSpriteLayer::createPlantsText(0, lta.find("GARLIC_1")->second->text, Vec2(190, 910), lta.find("GARLIC_1")->second->fontsize);
	SPSSpriteLayer::createPlantsText(2, lta.find("GARLIC_2")->second->text, Vec2(360, 1000), lta.find("GARLIC_2")->second->fontsize, Color3B::YELLOW, false);
	SPSSpriteLayer::createPlantsText(3, lta.find("GARLIC_3")->second->text, Vec2(440, 1000), lta.find("GARLIC_3")->second->fontsize, Color3B::RED, false);
	SPSSpriteLayer::createPlantsText(1, lta.find("GARLIC_4")->second->text, Vec2(360, 870), lta.find("GARLIC_4")->second->fontsize, Color3B::YELLOW, false);
	
	return _plantAnimation;
}
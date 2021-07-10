﻿/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.09
 *Email: 2117610943@qq.com
 */
#pragma once
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSPauseQuitLayer.h"

class HPauseQuitLayer :public GSPauseQuitLayer
{
public:
	CREATE_FUNC(HPauseQuitLayer);

CC_CONSTRUCTOR_ACCESS:
	HPauseQuitLayer();
	~HPauseQuitLayer();
	virtual bool init() override;

protected:
	virtual void createDialog() override;
	virtual void setRestart() override;
	virtual void setQuitGame() override;
};
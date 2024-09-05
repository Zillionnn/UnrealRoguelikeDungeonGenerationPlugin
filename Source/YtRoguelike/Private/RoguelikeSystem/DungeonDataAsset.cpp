// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeSystem/DungeonDataAsset.h"


UDungeonDataAsset::UDungeonDataAsset()
{
	MapWidth = 21;
	MapHeight = 21;
	MaxFeatures = 5;
	TileOffset = 20.f;
	bUseStairs = false;
	DungeonHeight = 200.0f;
}

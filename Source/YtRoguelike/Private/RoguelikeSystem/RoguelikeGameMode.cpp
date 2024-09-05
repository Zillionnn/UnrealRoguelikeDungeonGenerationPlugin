// Copyright Yntoo, 2024
// All Rights Reserved.

#include "RoguelikeSystem/RoguelikeGameMode.h"

#include "RoguelikeSystem/RoguelikeSubsystem.h"


void ARoguelikeGameMode::BeginPlay()
{
	Super::BeginPlay();
	auto RGS = GetGameInstance()->GetSubsystem<URoguelikeSubsystem>();
	if (RGS)
	{
		RGS->LoadDungeonDataAsset(DungeonDataAsset);
	}
}

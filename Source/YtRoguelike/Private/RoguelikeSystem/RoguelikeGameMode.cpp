// Fill out your copyright notice in the Description page of Project Settings.


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

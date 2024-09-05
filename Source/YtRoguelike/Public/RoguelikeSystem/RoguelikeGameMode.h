// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DungeonDataAsset.h"
#include "GameFramework/GameMode.h"
#include "RoguelikeGameMode.generated.h"

/**
 * 
 */
UCLASS()
class YTROGUELIKE_API ARoguelikeGameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Roguelike")
	TObjectPtr<UDungeonDataAsset> DungeonDataAsset;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DungeonDataAsset.generated.h"

class ARoguelikeTileActor;
/**
 * 
 */
UCLASS()
class YTROGUELIKE_API UDungeonDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Dungeon Settings")
	int32 MapWidth;

	UPROPERTY(EditAnywhere, Category = "Dungeon Settings")
	int32 MapHeight;

	UPROPERTY(EditAnywhere, Category = "Dungeon Settings")
	int32 MaxFeatures;

	UPROPERTY(EditAnywhere, Category = "Dungeon Settings")
	float TileOffset;

	UPROPERTY(EditAnywhere, Category = "Dungeon Settings")
	float DungeonHeight;


	UPROPERTY(EditAnywhere, Category = "Dungeon Settings")
	TSubclassOf<ARoguelikeTileActor> TileBPClass;


	UPROPERTY(EditAnywhere, Category = "Dungeon Settings")
	bool bDebug;

	UPROPERTY(EditAnywhere, Category = "Dungeon Settings")
	bool bUseStairs;

	// 构造函数
	UDungeonDataAsset();
};

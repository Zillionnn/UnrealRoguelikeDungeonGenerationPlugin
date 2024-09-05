// Copyright Yntoo, 2024
// All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "DungeonDataAsset.h"
#include "Subsystems/WorldSubsystem.h"
#include "RoguelikeSubsystem.generated.h"


class ARoguelikeTileActor;

USTRUCT()
struct FRect
{
	GENERATED_BODY()
	int x, y;
	int width, height;
};

UENUM()
enum ETile
{
	Unused,
	Floor,
	Corridor,
	Wall,
	ClosedDoor,
	OpenDoor,
	UpStairs,
	DownStairs
};

UENUM()
enum ERGDirection
{
	North,
	South,
	West,
	East,
	DirectionCount
};


USTRUCT(BlueprintType)
struct FRoguelikeTile
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<ETile> TileType;

	UPROPERTY(BlueprintReadOnly)
	int32 x;

	UPROPERTY(BlueprintReadOnly)
	int32 y;

	// 添加自定义构造函数（可选）
	FRoguelikeTile() : TileType(ETile::Unused), x(0), y(0)
	{
	}
};


/**
 * 
 */
UCLASS()
class YTROGUELIKE_API URoguelikeSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	UFUNCTION(BlueprintCallable)
	void LoadDungeonDataAsset(UDungeonDataAsset* InAsset);

	UFUNCTION(BlueprintCallable)
	void Generate();

	UFUNCTION(BlueprintCallable)
	void SpawnTiles();

	TArray<FRect> GetRooms();

	FVector GetRoomFirstTileLocation(FRect InRoom);

	int32 GetOriginX();
	int32 GetOriginY();
protected:
	bool MakeRoom(int x, int y, ERGDirection dir, bool firstRoom = false);

	bool CreateFeature();
	bool CreateFeature(int x, int y, ERGDirection dir);
	bool MakeCorridor(int x, int y, ERGDirection dir);
	bool PlaceRect(const FRect& rect, FRoguelikeTile tile);
	bool PlaceObject(FRoguelikeTile Tile);

private:
	FRoguelikeTile GetTile(int x, int y);
	void SetTile(int x, int y, FRoguelikeTile tile);

	ARoguelikeTileActor* SpawnTile(FTransform InTransform, ETile InTileType);

private:
	TArray<FRect> Rooms;
	TArray<FRect> Exits;

	UPROPERTY()
	TArray<FRoguelikeTile> Tiles;

	UPROPERTY()
	TObjectPtr<UDungeonDataAsset> DungeonDataAsset;
	bool bfirstRoom;

	int32 OriginX = -1;
	int32 OriginY = -1;
};

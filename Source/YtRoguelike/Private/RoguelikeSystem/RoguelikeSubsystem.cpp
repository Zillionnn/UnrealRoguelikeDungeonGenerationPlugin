// Copyright Yntoo, 2024
// All Rights Reserved.

#include "RoguelikeSystem/RoguelikeSubsystem.h"

#include "RoguelikeSystem/RoguelikeTileActor.h"
DEFINE_LOG_CATEGORY_STATIC(LogRogueSubSystem, All, All);


void URoguelikeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	bfirstRoom = false;
	OriginX = -1;
	OriginY = -1;
}

/**
 * 
 * @param InAsset
 */
void URoguelikeSubsystem::LoadDungeonDataAsset(UDungeonDataAsset* InAsset)
{
	bfirstRoom = false;
	OriginX = -1;
	OriginY = -1;

	if (InAsset)
	{
		DungeonDataAsset = InAsset;
		UE_LOG(LogRogueSubSystem, Log, TEXT(" --- load dungeon data asset success ----"));

		Tiles.Empty();

		for (int32 i = 0; i < DungeonDataAsset->MapHeight; i++)
		{
			for (int32 j = 0; j < DungeonDataAsset->MapWidth; j++)
			{
				FRoguelikeTile Tile;
				Tile.TileType = ETile::Unused;
				Tiles.Add(Tile);
			}
		}
		Generate();

		if (bfirstRoom)
		{
			SpawnTiles();
		}
	}
	else
	{
		// FALIED
	}
}

void URoguelikeSubsystem::Generate()
{
	if (!IsValid(DungeonDataAsset))
	{
		UE_LOG(LogRogueSubSystem, Error, TEXT(" no data asset"));
		return;
	}

	int32 MaxFeatures = DungeonDataAsset->MaxFeatures;
	int32 Width = DungeonDataAsset->MapWidth;
	int32 Height = DungeonDataAsset->MapHeight;


	// place the first room in the center
	int32 randInt = FMath::RandRange(0, 3);
	if (!MakeRoom(Width / 2, Height / 2, static_cast<ERGDirection>(randInt), true))
	{
		UE_LOG(LogRogueSubSystem, Error, TEXT("Unable to place the first room."));
		bfirstRoom = false;
		return;
	}

	bfirstRoom = true;
	// we already placed 1 feature (the first room)
	for (int i = 1; i < MaxFeatures; ++i)
	{
		if (!CreateFeature())
		{
			UE_LOG(LogRogueSubSystem, Warning, TEXT("Unable to place more features (placed  %d)."), i);
			break;
		}
	}

	if (DungeonDataAsset->bUseStairs)
	{
		FRoguelikeTile UpStairsTile;
		UpStairsTile.TileType = UpStairs;
		if (!PlaceObject(UpStairsTile))
		{
			UE_LOG(LogRogueSubSystem, Warning, TEXT("Unable to place up stairs."));
			return;
		}

		FRoguelikeTile DownStairsTile;
		DownStairsTile.TileType = DownStairs;
		if (!PlaceObject(DownStairsTile))
		{
			UE_LOG(LogRogueSubSystem, Warning, TEXT("Unable to place down stairs."));
			return;
		}
	}


	for (auto tile : Tiles)
	{
		// if (tile == Unused)
		// 	tile = '.';
		// else if (tile == Floor || tile == Corridor)
		// 	tile = ' ';


		// TODO 2024-09-04 20:42:00
		if (tile.TileType == Unused)
		{
			// tile = '.';
		}
		else if (tile.TileType == Floor || tile.TileType == Corridor)
		{
			// tile = ' ';  ?????
		}
	}
}

void URoguelikeSubsystem::SpawnTiles()
{
	int32 _height = DungeonDataAsset->MapHeight;
	int32 _width = DungeonDataAsset->MapWidth;

	for (int y = 0; y < _height; ++y)
	{
		for (int x = 0; x < _width; ++x)
		{
			FRoguelikeTile Tile = GetTile(x, y);
			if (Tile.TileType == ETile::Unused)
			{
				continue;
			}
			FTransform TileTransform;
			UE_LOG(LogRogueSubSystem, Log, TEXT("spawn index x  %d  y %d"), x, y);

			if (OriginX < 0 && OriginY < 0)
			{
				OriginX = x;
				OriginY = y;
			}


			TileTransform.SetLocation(FVector(x * DungeonDataAsset->TileOffset, y * DungeonDataAsset->TileOffset, 100.f));
			SpawnTile(TileTransform, Tile.TileType);
		}
	}
}

bool URoguelikeSubsystem::MakeRoom(int x, int y, ERGDirection dir, bool firstRoom)
{
	static const int minRoomSize = 3;
	static const int maxRoomSize = 6;

	FRect room;
	room.width = FMath::RandRange(minRoomSize, maxRoomSize);
	room.height = FMath::RandRange(minRoomSize, maxRoomSize);

	if (dir == North)
	{
		room.x = x - room.width / 2;
		room.y = y - room.height;
	}

	else if (dir == South)
	{
		room.x = x - room.width / 2;
		room.y = y + 1;
	}

	else if (dir == West)
	{
		room.x = x - room.width;
		room.y = y - room.height / 2;
	}

	else if (dir == East)
	{
		room.x = x + 1;
		room.y = y - room.height / 2;
	}


	FRoguelikeTile FloorTile;
	FloorTile.TileType = ETile::Floor;
	if (PlaceRect(room, FloorTile))
	{
		Rooms.Emplace(room);

		if (dir != South || firstRoom) // north side
			Exits.Emplace(FRect{room.x, room.y - 1, room.width, 1});
		if (dir != North || firstRoom) // south side
			Exits.Emplace(FRect{room.x, room.y + room.height, room.width, 1});
		if (dir != East || firstRoom) // west side
			Exits.Emplace(FRect{room.x - 1, room.y, 1, room.height});
		if (dir != West || firstRoom) // east side
			Exits.Emplace(FRect{room.x + room.width, room.y, 1, room.height});

		return true;
	}

	return false;
}

bool URoguelikeSubsystem::CreateFeature()
{
	for (int i = 0; i < 1000; ++i)
	{
		if (Exits.IsEmpty())
			break;

		// choose a random side of a random room or corridor

		int r = FMath::RandRange(0, Exits.Num() - 1);
		int x = FMath::RandRange(Exits[r].x, Exits[r].x + Exits[r].width - 1);
		int y = FMath::RandRange(Exits[r].y, Exits[r].y + Exits[r].height - 1);

		// north, south, west, east
		for (int j = 0; j < DirectionCount; ++j)
		{
			if (CreateFeature(x, y, static_cast<ERGDirection>(j)))
			{
				if (Exits.IsValidIndex(r))
				{
					Exits.RemoveAt(r);
				}
				return true;
			}
		}
	}

	return false;
}

bool URoguelikeSubsystem::CreateFeature(int x, int y, ERGDirection dir)
{
	static const int roomChance = 50; // corridorChance = 100 - roomChance

	int dx = 0;
	int dy = 0;

	if (dir == North)
		dy = 1;
	else if (dir == South)
		dy = -1;
	else if (dir == West)
		dx = 1;
	else if (dir == East)
		dx = -1;

	if (GetTile(x + dx, y + dy).TileType != ETile::Floor && GetTile(x + dx, y + dy).TileType != ETile::Corridor)
		return false;

	if (FMath::RandRange(0, 99) < roomChance)
	{
		if (MakeRoom(x, y, dir))
		{
			FRoguelikeTile DoorTile;
			DoorTile.x = x;
			DoorTile.y = y;
			DoorTile.TileType = ClosedDoor;
			SetTile(x, y, DoorTile);

			return true;
		}
	}

	else
	{
		if (MakeCorridor(x, y, dir))
		{
			if (GetTile(x + dx, y + dy).TileType == Floor)
			{
				FRoguelikeTile ClosedDoorTile;
				ClosedDoorTile.x = x;
				ClosedDoorTile.y = y;
				ClosedDoorTile.TileType = ClosedDoor;
				SetTile(x, y, ClosedDoorTile);
			}

			else
			{
				FRoguelikeTile CorridorTile;
				CorridorTile.x = x;
				CorridorTile.y = y;
				CorridorTile.TileType = ETile::Corridor;
				// don't place a door between corridors
				SetTile(x, y, CorridorTile);
			}

			return true;
		}
	}

	return false;
}

bool URoguelikeSubsystem::MakeCorridor(int x, int y, ERGDirection dir)
{
	const int minCorridorLength = 3;
	const int maxCorridorLength = 6;

	FRect corridor;
	corridor.x = x;
	corridor.y = y;

	if (FMath::RandBool()) // horizontal corridor
	{
		corridor.width = FMath::RandRange(minCorridorLength, maxCorridorLength);
		corridor.height = 1;

		if (dir == North)
		{
			corridor.y = y - 1;

			if (FMath::RandBool()) // west
				corridor.x = x - corridor.width + 1;
		}

		else if (dir == South)
		{
			corridor.y = y + 1;

			if (FMath::RandBool()) // west
				corridor.x = x - corridor.width + 1;
		}

		else if (dir == West)
			corridor.x = x - corridor.width;

		else if (dir == East)
			corridor.x = x + 1;
	}

	else // vertical corridor
	{
		corridor.width = 1;
		corridor.height = FMath::RandRange(minCorridorLength, maxCorridorLength);

		if (dir == North)
			corridor.y = y - corridor.height;

		else if (dir == South)
			corridor.y = y + 1;

		else if (dir == West)
		{
			corridor.x = x - 1;

			if (FMath::RandBool()) // north
				corridor.y = y - corridor.height + 1;
		}

		else if (dir == East)
		{
			corridor.x = x + 1;

			if (FMath::RandBool()) // north
				corridor.y = y - corridor.height + 1;
		}
	}

	FRoguelikeTile CorridorTile;
	CorridorTile.TileType = ETile::Corridor;

	if (PlaceRect(corridor, CorridorTile))
	{
		if (dir != South && corridor.width != 1) // north side
			Exits.Emplace(FRect{corridor.x, corridor.y - 1, corridor.width, 1});
		if (dir != North && corridor.width != 1) // south side
			Exits.Emplace(FRect{corridor.x, corridor.y + corridor.height, corridor.width, 1});
		if (dir != East && corridor.height != 1) // west side
			Exits.Emplace(FRect{corridor.x - 1, corridor.y, 1, corridor.height});
		if (dir != West && corridor.height != 1) // east side
			Exits.Emplace(FRect{corridor.x + corridor.width, corridor.y, 1, corridor.height});

		return true;
	}

	return false;
}

bool URoguelikeSubsystem::PlaceRect(const FRect& rect, FRoguelikeTile tile)
{
	int32 _width = DungeonDataAsset->MapWidth;
	int32 _height = DungeonDataAsset->MapHeight;

	if (rect.x < 1 || rect.y < 1 || rect.x + rect.width > _width - 1 || rect.y + rect.height > _height - 1)
		return false;

	for (int y = rect.y; y < rect.y + rect.height; ++y)
	{
		for (int x = rect.x; x < rect.x + rect.width; ++x)
		{
			if (GetTile(x, y).TileType != Unused)
				return false; // the area already used
		}
	}

	for (int y = rect.y - 1; y < rect.y + rect.height + 1; ++y)
	{
		for (int x = rect.x - 1; x < rect.x + rect.width + 1; ++x)
		{
			if (x == rect.x - 1 || y == rect.y - 1 || x == rect.x + rect.width || y == rect.y + rect.height)
			{
				FRoguelikeTile WallTile;
				WallTile.x = x;
				WallTile.y = y;
				WallTile.TileType = ETile::Wall;
				SetTile(x, y, WallTile);
			}
			else
			{
				SetTile(x, y, tile);
			}
		}
	}
	return true;
}

bool URoguelikeSubsystem::PlaceObject(FRoguelikeTile InTile)
{
	if (Rooms.IsEmpty())
		return false;

	int r = FMath::RandRange(0, Rooms.Num() - 1); // choose a random room
	int x = FMath::RandRange(Rooms[r].x + 1, Rooms[r].x + Rooms[r].width - 2);
	int y = FMath::RandRange(Rooms[r].y + 1, Rooms[r].y + Rooms[r].height - 2);

	if (GetTile(x, y).TileType == Floor)
	{
		SetTile(x, y, InTile);

		// place one object in one room (optional)
		Rooms.RemoveAt(r);

		return true;
	}

	return false;
}

TArray<FRect> URoguelikeSubsystem::GetRooms()
{
	return Rooms;
}

FVector URoguelikeSubsystem::GetRoomFirstTileLocation(FRect InRoom)
{
	float Offset = DungeonDataAsset->TileOffset;
	const float x = InRoom.x * Offset - GetOriginX() * Offset;
	const float y = InRoom.y * Offset - GetOriginY() * Offset;

	return FVector(x, y, DungeonDataAsset->DungeonHeight);
}

int32 URoguelikeSubsystem::GetOriginX()
{
	return OriginX;
}

int32 URoguelikeSubsystem::GetOriginY()
{
	return OriginY;
}


FRoguelikeTile URoguelikeSubsystem::GetTile(int x, int y)
{
	// UE_LOG(LogRogueSubSystem, Log, TEXT("%s >> x   %d  y %d  sum %d"), *FString(__FUNCTION__), x, y, Tiles.Num());

	if (x < 0 || y < 0 || x >= DungeonDataAsset->MapWidth || y >= DungeonDataAsset->MapHeight)
	{
		FRoguelikeTile Tile;
		Tile.x = x;
		Tile.y = y;
		Tile.TileType = ETile::Unused;
		return Tile;
	}


	return Tiles[x + y * DungeonDataAsset->MapWidth];
}

void URoguelikeSubsystem::SetTile(int x, int y, FRoguelikeTile tile)
{
	Tiles[x + y * DungeonDataAsset->MapWidth] = tile;
}

ARoguelikeTileActor* URoguelikeSubsystem::SpawnTile(FTransform InTransform, ETile InTileType)
{
	if (!IsValid(DungeonDataAsset))
	{
		return nullptr;
	}

	if (DungeonDataAsset->TileBPClass)
	{
		ARoguelikeTileActor* TileActor = GetWorld()->SpawnActorDeferred<ARoguelikeTileActor>(DungeonDataAsset->TileBPClass, InTransform);

		if (TileActor)
		{
			TileActor->TileType = InTileType;
			TileActor->bDebug = DungeonDataAsset->bDebug;
			TileActor->TileOffset = DungeonDataAsset->TileOffset;

			const float x = InTransform.GetLocation().X - OriginX * DungeonDataAsset->TileOffset;
			const float y = InTransform.GetLocation().Y - OriginY * DungeonDataAsset->TileOffset;

			InTransform.SetLocation(FVector(x, y, DungeonDataAsset->DungeonHeight));
			TileActor->FinishSpawning(InTransform);
			return TileActor;
		}
		else
		{
			UE_LOG(LogRogueSubSystem, Error, TEXT("Spawn Tile ERROR"));
		}
	}
	return nullptr;
}

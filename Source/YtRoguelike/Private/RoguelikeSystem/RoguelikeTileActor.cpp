// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeSystem/RoguelikeTileActor.h"

#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"

// Sets default values
ARoguelikeTileActor::ARoguelikeTileActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = Box;


	TextRender = CreateDefaultSubobject<UTextRenderComponent>("TextRender");
	TextRender->SetupAttachment(RootComponent);

	// (Pitch=90.000000,Yaw=0.000000,Roll=0.000000)
	TextRender->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));


	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
}


//
// Unused = '.',
// Floor = ' ',
// Corridor = ',',
// Wall = '#',
// ClosedDoor = '+',
// OpenDoor = '-',
// UpStairs = '<',
// DownStairs = '>'

// Called when the game starts or when spawned
void ARoguelikeTileActor::BeginPlay()
{
	Super::BeginPlay();
	if (bDebug)
	{
		switch (TileType)
		{
		case ETile::Floor:
			{
				TextRender->SetText(FText::FromString(TEXT(" ")));


				break;
			}
		case ETile::Corridor:
			{
				TextRender->SetText(FText::FromString(TEXT(",")));
				break;
			}
		case ETile::Wall:
			{
				TextRender->SetText(FText::FromString(TEXT("#")));
				break;
			}

		case ETile::ClosedDoor:
			{
				TextRender->SetText(FText::FromString(TEXT("+")));
				break;
			}

		case ETile::OpenDoor:
			{
				TextRender->SetText(FText::FromString(TEXT("-")));
				break;
			}


		case ETile::UpStairs:
			{
				TextRender->SetText(FText::FromString(TEXT("<")));
				break;
			}


		case ETile::DownStairs:
			{
				TextRender->SetText(FText::FromString(TEXT(">")));
				break;
			}


		default: break;
		}
	}
	else
	{
		TextRender->SetHiddenInGame(true);
	}


	SetMesh();
}

void ARoguelikeTileActor::SetMesh()
{
	switch (TileType)
	{
	case ETile::Wall:
		{
			if (WallMesh)
			{
				MeshComponent->SetStaticMesh(WallMesh);
			}
			break;
		}
	case ETile::Floor:
		{
			if (FloorMesh)
			{
				MeshComponent->SetStaticMesh(FloorMesh);
			}
			break;
		}
	case ETile::Corridor:
		{
			if (CorridorMesh)
			{
				MeshComponent->SetStaticMesh(CorridorMesh);
			}
			break;
		}

	case ETile::ClosedDoor:
		{
			if (ClosedDoor)
			{
				MeshComponent->SetStaticMesh(ClosedDoorMesh);
			}
			break;
		}

	case ETile::OpenDoor:
		{
			if (OpenDoor)
			{
				MeshComponent->SetStaticMesh(OpenDoorMesh);
			}
			break;
		}
	default: break;
	}
}

// Called every frame
void ARoguelikeTileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

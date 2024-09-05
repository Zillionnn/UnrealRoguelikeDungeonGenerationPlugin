// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoguelikeSubsystem.h"
#include "GameFramework/Actor.h"
#include "RoguelikeTileActor.generated.h"


class UBoxComponent;

UCLASS()
class YTROGUELIKE_API ARoguelikeTileActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARoguelikeTileActor();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UBoxComponent> Box;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<class UTextRenderComponent> TextRender;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetMesh();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	UPROPERTY(EditAnywhere, Category="TileConfig")
	TObjectPtr<UStaticMesh> FloorMesh;

	UPROPERTY(EditAnywhere, Category="TileConfig")
	TObjectPtr<UStaticMesh> CorridorMesh;

	UPROPERTY(EditAnywhere, Category="TileConfig")
	TObjectPtr<UStaticMesh> WallMesh;

	UPROPERTY(EditAnywhere, Category="TileConfig")
	TObjectPtr<UStaticMesh> ClosedDoorMesh;

	UPROPERTY(EditAnywhere, Category="TileConfig")
	TObjectPtr<UStaticMesh> OpenDoorMesh;

	bool bDebug;
	ETile TileType;
	float TileOffset;

};

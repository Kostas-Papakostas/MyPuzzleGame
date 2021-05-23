// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Engine.h"
#include "PlayersProjectile.h"
#include "PuzzleBlocker.generated.h"

UCLASS()
class MYPUZZLEGAME_API APuzzleBlocker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APuzzleBlocker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BlockersMesh)
		UStaticMeshComponent* blockerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MainAttributes)
		TEnumAsByte<EPlayersProjectileColor> blockerColor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MainAttributes)
		TArray<UMaterialInterface*> blockerColorTable;
	
};

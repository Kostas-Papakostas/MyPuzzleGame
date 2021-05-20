// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Engine.h"
#include "PuzzleProjectile.h"
#include "PuzzleBlocker.generated.h"

UCLASS()
class MYPUZZLEGAME_API APuzzleBlocker : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* mainBody;
	
public:	
	// Sets default values for this actor's properties
	APuzzleBlocker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};

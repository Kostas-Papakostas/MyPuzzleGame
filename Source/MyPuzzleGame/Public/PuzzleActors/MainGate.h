// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainGate.generated.h"

UCLASS()
class MYPUZZLEGAME_API AMainGate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMainGate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		uint16 keysGathered;

	UPROPERTY(EditAnywhere)
		uint16 totalKeysToUnlock;

	UPROPERTY(EditAnywhere)
		bool doorLocked;
	
	
};

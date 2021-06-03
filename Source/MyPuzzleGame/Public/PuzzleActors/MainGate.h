// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainGate.generated.h"

UCLASS()
class MYPUZZLEGAME_API AMainGate : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* mainFrame;

	FTimerHandle checkTheKeys;

	TArray<AActor*> targetActors;

public:	
	// Sets default values for this actor's properties
	AMainGate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unlock Attributes")
		int keysGathered;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unlock Attributes")
		int totalKeysToUnlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unlock Attributes")
		bool doorUnLocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meshes")
		UStaticMeshComponent* mainGate;
	
	UFUNCTION()
		void checkIfUnlocked();

	
};

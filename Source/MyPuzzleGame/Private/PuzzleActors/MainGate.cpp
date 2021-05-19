// Fill out your copyright notice in the Description page of Project Settings.

#include "MainGate.h"


// Sets default values
AMainGate::AMainGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMainGate::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (keysGathered >= totalKeysToUnlock) {
		doorLocked = true;
	}
}


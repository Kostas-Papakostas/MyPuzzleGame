// Fill out your copyright notice in the Description page of Project Settings.

#include "TeleportGate.h"


// Sets default values
ATeleportGate::ATeleportGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATeleportGate::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATeleportGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


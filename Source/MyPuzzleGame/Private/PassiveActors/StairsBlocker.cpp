// Fill out your copyright notice in the Description page of Project Settings.

#include "StairsBlocker.h"


// Sets default values
AStairsBlocker::AStairsBlocker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStairsBlocker::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStairsBlocker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

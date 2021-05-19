// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileSource.h"


// Sets default values
AProjectileSource::AProjectileSource()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProjectileSource::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileSource::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


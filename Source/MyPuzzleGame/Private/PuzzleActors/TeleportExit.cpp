// Fill out your copyright notice in the Description page of Project Settings.

#include "TeleportExit.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATeleportExit::ATeleportExit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	frameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Frame"));
	frameMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	frameMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	vortexMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Vortex"));
	vortexMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	vortexMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

}

// Called when the game starts or when spawned
void ATeleportExit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATeleportExit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


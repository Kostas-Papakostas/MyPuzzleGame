// Fill out your copyright notice in the Description page of Project Settings.

#include "MainReflector.h"
#include "Engine/Engine.h"
#include "Components/StaticMeshComponent.h"



// Sets default values
AMainReflector::AMainReflector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bIsFloating = false;

	mainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Main Reflector"));
	mainMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	mainMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	mainMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

}

// Called when the game starts or when spawned
void AMainReflector::BeginPlay()
{
	Super::BeginPlay();
	mainMesh->SetMaterial(0, mainMeshMat);
}

// Called every frame
void AMainReflector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Yellow, TEXT("I'M HERE 2"));
	}

	if (bIsFloating)
		mainMesh->SetMaterial(0, mainMeshFloatingMat);
	else
		mainMesh->SetMaterial(0, mainMeshMat);

}


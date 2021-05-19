// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileReflector.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AProjectileReflector::AProjectileReflector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mainBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Main Body"));
	mainBody->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	mainBody->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	mainBody->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	RootComponent = mainBody;

	bounceArea = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bounce Area"));
	bounceArea->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	bounceArea->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	bounceArea->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	
	reflector = CreateDefaultSubobject<UChildActorComponent>(TEXT("Reflector Area"));

	bounceArea->SetupAttachment(RootComponent);
	reflector->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectileReflector::BeginPlay()
{
	Super::BeginPlay();
	mainBody->SetMaterial(0, mainBodyMaterial);
	bounceArea->SetMaterial(0, bounceAreaMaterial);
}

// Called every frame
void AProjectileReflector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileReflector.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ChildActorComponent.h"



// Sets default values
AProjectileReflector::AProjectileReflector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	floating = false;

	overallBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Overall Box"));
	overallBox->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	overallBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	overallBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	overallBox->SetConstraintMode(EDOFMode::Default);

	RootComponent = overallBox;

	mainBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Main Body"));
	mainBody->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	mainBody->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	mainBody->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	mainBody->SetupAttachment(RootComponent);
	//RootComponent = mainBody;

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
	overallBox->SetSimulatePhysics(true);
	mainBody->SetSimulatePhysics(false);
	overallBox->BodyInstance.bLockXRotation = true;
	overallBox->BodyInstance.bLockYRotation = true;
	overallBox->BodyInstance.bLockZRotation = true;
	overallBox->SetAngularDamping(5.f);
	overallBox->SetLinearDamping(5.f);

	mainBody->SetMaterial(0, mainBodyMaterial);
	bounceArea->SetMaterial(0, bounceAreaMaterial);
}

// Called every frame
void AProjectileReflector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	this->SetActorRotation(FRotator::FRotator(0, this->GetActorRotation().Yaw, 0));
}


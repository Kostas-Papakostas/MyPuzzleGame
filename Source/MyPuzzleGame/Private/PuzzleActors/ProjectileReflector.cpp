// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileReflector.h"
#include "PuzzleProjectile.h"
#include "Engine.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ChildActorComponent.h"



// Sets default values
AProjectileReflector::AProjectileReflector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	floating = false;

	mainBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Main Body"));
	mainBody->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	mainBody->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	mainBody->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	//mainBody->SetupAttachment(RootComponent);
	RootComponent = mainBody;

	overallBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Overall Box"));
	overallBox->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	overallBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	overallBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	FCollisionResponseContainer newContainer;
	newContainer.SetAllChannels(ECollisionResponse::ECR_Overlap);
	newContainer.SetResponse(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Block);
	newContainer.SetResponse(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	newContainer.SetResponse(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	newContainer.SetResponse(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

	overallBox->SetCollisionResponseToChannels(newContainer);
	overallBox->SetConstraintMode(EDOFMode::Default);
	overallBox->SetupAttachment(RootComponent);

	overallBox->bMultiBodyOverlap = true;

	bounceArea = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bounce Area"));
	bounceArea->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	bounceArea->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	bounceArea->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	
	reflector = CreateDefaultSubobject<UChildActorComponent>(TEXT("Reflector Area"));

	bounceArea->SetupAttachment(RootComponent);
	reflector->SetupAttachment(RootComponent);
	overallBox->OnComponentBeginOverlap.AddDynamic(this, &AProjectileReflector::OnOverlap);
}

// Called when the game starts or when spawned
void AProjectileReflector::BeginPlay()
{
	Super::BeginPlay();
	/*some collision and physics settings*/
	overallBox->SetSimulatePhysics(false);
	mainBody->SetSimulatePhysics(true);
	mainBody->SetNotifyRigidBodyCollision(false);
	mainBody->SetEnableGravity(true);
	mainBody->BodyInstance.bLockXRotation = true;
	mainBody->BodyInstance.bLockYRotation = true;
	mainBody->BodyInstance.bLockZRotation = true;
	mainBody->SetAngularDamping(5.f);
	mainBody->SetLinearDamping(5.f);

	mainBody->SetMaterial(0, mainBodyMaterial);
	bounceArea->SetMaterial(0, bounceAreaMaterial);

	initialLocation = this->GetActorLocation();	

}

/*used to keep yaw and roll steady*/
void AProjectileReflector::gyroscopicRotation_Implementation()
{
	currentRotation = this->GetActorRotation();
	this->SetActorRotation(FRotator::FRotator(0, this->GetActorRotation().Yaw, 0));
}

// Called every frame
void AProjectileReflector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	gyroscopicRotation_Implementation();
	/*if picked up change color*/
	if (floating) {
		mainBody->SetMaterial(0, mainBodyFloatingMaterial);
		bounceArea->SetMaterial(0, bounceAreaFloatingMaterial);
	}
	else {
		mainBody->SetMaterial(0, mainBodyMaterial);
		bounceArea->SetMaterial(0, bounceAreaMaterial);
	}

	if (this->GetActorLocation().Z <= -200.f) {
		this->TeleportTo(initialLocation, currentRotation);
	}
}

void AProjectileReflector::OnOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	APuzzleProjectile* projectileToDestroy = Cast<APuzzleProjectile>(OtherActor);
	if (projectileToDestroy)
		OtherActor->Destroy();
}


// Fill out your copyright notice in the Description page of Project Settings.

#include "TeleportGate.h"
#include "TeleportExit.h"
#include "PuzzleProjectile.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ATeleportGate::ATeleportGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	frameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Frame"));
	frameMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	frameMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	RootComponent = frameMesh;

	vortexMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Vortex"));
	vortexMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	vortexMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	vortexMesh->SetupAttachment(RootComponent);

	overlapComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Projectile Checker"));
	overlapComponent->BodyInstance.SetCollisionProfileName("Overlaper");
	overlapComponent->SetGenerateOverlapEvents(true);
	overlapComponent->SetupAttachment(vortexMesh);
	overlapComponent->OnComponentBeginOverlap.AddDynamic(this, &ATeleportGate::OnOverlap);

	gateID = 0;
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

void ATeleportGate::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TArray<AActor*> exitGates;
	UWorld* const World = GetWorld();
	UGameplayStatics::GetAllActorsOfClass(World, ATeleportExit::StaticClass(), exitGates);
	APuzzleProjectile* overlappedProjectile = dynamic_cast<APuzzleProjectile*>(OtherActor);
	FVector exitForwardVector;

	if (overlappedProjectile) {
		/*if the overlap object is a projectile then teleport to the corresponding gate*/
		for(AActor* myGate:exitGates){
			if (Cast<ATeleportExit>(myGate)->id == gateID) {
				exitForwardVector = myGate->GetActorRightVector();
				float tpHeight = this->GetActorLocation().Z - overlappedProjectile->GetActorLocation().Z;
				overlappedProjectile->GetProjectileMovement()->Velocity = exitForwardVector.GetSafeNormal()*overlappedProjectile->GetProjectileMovement()->MaxSpeed;
				//teleported = overlappedProjectile->TeleportTo(myGate->GetActorLocation(), overlappedProjectile->GetActorRotation());
				teleported = overlappedProjectile->TeleportTo(FVector::FVector(myGate->GetActorLocation().X, myGate->GetActorLocation().Y, myGate->GetActorLocation().Z-tpHeight), overlappedProjectile->GetActorRotation());
			}
		}
	}
}


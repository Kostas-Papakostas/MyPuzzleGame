// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayersProjectile.h"
#include "PlayerActors/MyPuzzleGameCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PuzzleBlocker.h"

/*THE PROJECTILE A PLAYER SHOOT*/
// Sets default values
APlayersProjectile::APlayersProjectile()
{

	PrimaryActorTick.bCanEverTick = true;

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &APlayersProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	sphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	sphereMesh->SetRelativeScale3D(FVector(0.1, 0.1, 0.1));
	sphereMesh->SetupAttachment(RootComponent);

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void APlayersProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UWorld* const World = GetWorld();
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL)) {
		APuzzleBlocker* tempBlocker = Cast<APuzzleBlocker>(OtherActor);

		if (tempBlocker && tempBlocker->blockerColor==color) {//if projectile hits none of them then destroy
			OtherActor->Destroy();
			// try and play the sound if specified
			if (destroySound != NULL)
			{
				UGameplayStatics::PlaySoundAtLocation(this, destroySound, UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation());
			}
			this->Destroy();
		}
		else {
			this->Destroy();
		}
	}
}
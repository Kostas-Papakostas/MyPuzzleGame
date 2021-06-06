// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileSource.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Engine.h"


// Sets default values
AProjectileSource::AProjectileSource()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mainBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Main Body Comp"));
	mainBody->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	mainBody->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	mainBody->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	mainBody->SetupAttachment(RootComponent);

	spawnPointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Spoint Point Comp"));
	spawnPointMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	spawnPointMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	spawnPointMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	spawnPointMesh->SetupAttachment(mainBody);

	spawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("CarryObjectLocation"));
	spawnPoint->SetupAttachment(spawnPointMesh);
	spawnPoint->SetRelativeLocation(FVector(0.2f, 230.0f, 32.0f));
}

// Called when the game starts or when spawned
void AProjectileSource::BeginPlay()
{
	Super::BeginPlay();
	if (spawnEdgeMaterials.Num() > 0) {
		mainBody->SetMaterial(0, bodyMaterial);
		spawnPointMesh->SetMaterial(0, spawnEdgeMaterials.operator[](spawnMaterial));
	}
	/*sets a timer to spawn a projectile every x seconds*/
	GetWorld()->GetTimerManager().SetTimer(timerShootingProjectile, this, &AProjectileSource::SpawnBall, 2.f, true);
}

// Called every frame
void AProjectileSource::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileSource::SpawnBall()
{
	APuzzleProjectile* projectile = GetWorld()->SpawnActor<APuzzleProjectile>(ProjectileClass, spawnPoint->GetComponentLocation(), spawnPoint->GetComponentRotation());
	projectile->color = spawnMaterial;
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzleProjectileTarget.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "MainGate.h"

// Sets default values
APuzzleProjectileTarget::APuzzleProjectileTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	outerTargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Outer Mesh"));
	outerTargetMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	outerTargetMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	outerTargetMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	outerTargetMesh->SetupAttachment(RootComponent);

	innerTargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Inner Mesh"));
	innerTargetMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	innerTargetMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	innerTargetMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	innerTargetMesh->SetupAttachment(outerTargetMesh);
	innerTargetMesh->OnComponentHit.AddDynamic(this, &APuzzleProjectileTarget::OnHit);


}



// Called when the game starts or when spawned
void APuzzleProjectileTarget::BeginPlay()
{
	Super::BeginPlay();
	if (innerMeshMaterials.Num() >color && outerMeshMaterials.Num()>color ) {
		outerTargetMesh->SetMaterial(0, outerMeshMaterials.operator[](color));
		innerTargetMesh->SetMaterial(0, innerMeshMaterials.operator[](color));
	}
}

// Called every frame
void APuzzleProjectileTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APuzzleProjectileTarget::OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	UWorld* const  World = GetWorld();
	if (OtherActor) {
		APuzzleProjectile* tempP = Cast<APuzzleProjectile>(OtherActor);
		if (tempP) {
			if (tempP->color.operator==(color)&&!noMoreKeys) {
				keys++;
				//increment A global Variable indicating progress
				TArray<AActor*> FoundActors;
				UGameplayStatics::GetAllActorsOfClass(World, AMainGate::StaticClass(), FoundActors);
				if (FoundActors.Num()>0) {
					Cast<AMainGate>(FoundActors.operator[](0))->keysGathered++;
				}
				noMoreKeys = !noMoreKeys;
				OtherActor->Destroy();
			}
		}
	}

}


// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzleBlocker.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"

/*just blocks the projectile*/
// Sets default values
APuzzleBlocker::APuzzleBlocker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	blockerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Main Wall"));
	blockerMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	blockerMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	blockerMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	RootComponent = blockerMesh;

}

// Called when the game starts or when spawned
void APuzzleBlocker::BeginPlay()
{
	Super::BeginPlay();
	if (blockerColor >= 0 && blockerColor < blockerColorTable.Num()) {
		blockerMesh->SetMaterial(0, blockerColorTable.operator[](blockerColor));
	}
}
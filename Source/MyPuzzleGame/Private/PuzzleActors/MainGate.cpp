// Fill out your copyright notice in the Description page of Project Settings.

#include "MainGate.h"
#include "PlayerActors/MyPuzzleGameCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "PuzzleProjectileTarget.h"
#include "Engine.h"

// Sets default values
AMainGate::AMainGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mainFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Main Frame"));
	mainFrame->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	mainFrame->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	mainFrame->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	RootComponent = mainFrame;

	mainGate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Main Wall"));
	mainGate->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	mainGate->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	mainGate->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	mainGate->SetupAttachment(mainFrame);

}

// Called when the game starts or when spawned
void AMainGate::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APuzzleProjectileTarget::StaticClass(), targetActors);
	totalKeysToUnlock = targetActors.Num();
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, FString::FromInt(totalKeysToUnlock));

	/*sets timer to check if all keys unlocked, it checks every x seconds*/
	GetWorld()->GetTimerManager().SetTimer(checkTheKeys, this, &AMainGate::checkIfUnlocked, 2.f, true);
}

// Called every frame
void AMainGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMainGate::checkIfUnlocked() {

	for (AActor* myActor : targetActors) {
		APuzzleProjectileTarget* myTarget = Cast<APuzzleProjectileTarget>(myActor);
		keysGathered += myTarget->keys;
	}

	if (keysGathered >= totalKeysToUnlock) {
		// try and play the sound if specified
		if (unlockSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, unlockSound, UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation());
		}
		doorUnLocked = true;
	}
	else
		keysGathered = 0;

	if (doorUnLocked) {
		/*if door gets unlocked destroy the main mesh */
		ACharacter* myCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		AMyPuzzleGameCharacter* puzzleCharacter = Cast<AMyPuzzleGameCharacter>(myCharacter);
		if (puzzleCharacter)
			puzzleCharacter->changeMaterial();
		mainFrame->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		if(mainGate)
			mainGate->DestroyComponent();
	}
}


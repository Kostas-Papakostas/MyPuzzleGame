// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MyPuzzleGameCharacter.h"
#include "MyPuzzleGameProjectile.h"
#include "PuzzleProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "ProjectileReflector.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Components/BoxComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "MainReflector.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AMyPuzzleGameCharacter

AMyPuzzleGameCharacter::AMyPuzzleGameCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GravityGunOn = false;
	bRotateLeft = false;
	bRotateRight = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	distance = 300.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	pickUpBeam = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Pick Up Beam"));
	pickUpBeam->SetupAttachment(FP_MuzzleLocation);

	GG_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("CarryObjectLocation"));
	GG_MuzzleLocation->SetupAttachment(FP_MuzzleLocation);
	GG_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 300.f, 32.0f));
	
	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

}


void AMyPuzzleGameCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	Mesh1P->SetHiddenInGame(false, true);
	pickUpBeam->SetColorParameter("BaseColor", FLinearColor::Green);
}

void AMyPuzzleGameCharacter::Tick(float DeltaTime)
{
	muzzleLocation = FP_MuzzleLocation->GetComponentLocation();
	muzzleForwardVector = FP_MuzzleLocation->GetRightVector();
	endPoint = muzzleLocation + (muzzleForwardVector*distance);
	
	if (GravityGunOn) {
		DrawDebugLine(GetWorld(), muzzleLocation, endPoint, FColor::Blue, false, 2.0f);
		
		if(Hit.bBlockingHit)
			DrawDebugPoint(GetWorld(), Hit.ImpactPoint, 5.0f, FColor::Red, true);

		if (reflectorActor) {
			
			IPickupable* pickableActor = dynamic_cast<IPickupable*>(reflectorActor);
			if (pickableActor) {
				reflectorActor->mainBody->SetSimulatePhysics(false);
				reflectorActor->mainBody->SetEnableGravity(false);
				reflectorActor->AttachToComponent(GG_MuzzleLocation, FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false));
				reflectorActor->floating = true;
				pickUpBeam->SetColorParameter("BaseColor", FLinearColor::FLinearColor(1,0,0));
				AMainReflector* tempReflector = Cast<AMainReflector>(reflectorActor->reflector->GetChildActor());
				if (tempReflector) {
					tempReflector->bIsFloating = true;
				}
			}
		}
		else
			GravityGunOn = false;
	}

	if (reflectorActor) {
		if (bRotateLeft)
			reflectorActor->AddActorWorldRotation(FRotator::FRotator(0, 1, 0));
		else if(bRotateRight)
			reflectorActor->AddActorWorldRotation(FRotator::FRotator(0, -1, 0));
	}


}

void AMyPuzzleGameCharacter::changeMaterial()
{
	FP_Gun->SetMaterial(0, FP_Gun_Mat);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMyPuzzleGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("ToggleGravityGun", IE_Pressed, this, &AMyPuzzleGameCharacter::UseGravityGun);
	
	/*Rotate Left or stop*/
	PlayerInputComponent->BindAction("RotateLeft", IE_Pressed, this, &AMyPuzzleGameCharacter::RotateLeft);
	PlayerInputComponent->BindAction("RotateLeft", IE_Released, this, &AMyPuzzleGameCharacter::RotateLeft);
	
	/*Rotate Right or Stop*/
	PlayerInputComponent->BindAction("RotateRight", IE_Pressed, this, &AMyPuzzleGameCharacter::RotateRight);
	PlayerInputComponent->BindAction("RotateRight", IE_Released, this, &AMyPuzzleGameCharacter::RotateRight);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("BlackFire", IE_Pressed, this, &AMyPuzzleGameCharacter::OnBlackFire);
	PlayerInputComponent->BindAction("WhiteFire", IE_Pressed, this, &AMyPuzzleGameCharacter::OnWhiteFire);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyPuzzleGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyPuzzleGameCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMyPuzzleGameCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMyPuzzleGameCharacter::LookUpAtRate);
}

void AMyPuzzleGameCharacter::OnBlackFire()
{
	// try and fire a projectile
	if (blackProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			if (bUsingMotionControllers)
			{
				const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
				const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
				World->SpawnActor<APlayersProjectile>(blackProjectileClass, SpawnLocation, SpawnRotation);
			}
			else
			{
				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// spawn the projectile at the muzzle
				APlayersProjectile* tempP = World->SpawnActor<APlayersProjectile>(blackProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
				if (tempP) {
					if (GEngine)
						GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, TEXT("SPAWNED"));
				}
				else {
					if (GEngine)
						GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, TEXT("ERROR"));
				}
			}
		}
	}else
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Yellow, TEXT("ERROR"));


	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void AMyPuzzleGameCharacter::OnWhiteFire()
{
	// try and fire a projectile
	if (whiteProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			if (bUsingMotionControllers)
			{
				const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
				const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
				World->SpawnActor<APlayersProjectile>(whiteProjectileClass, SpawnLocation, SpawnRotation);
			}
			else
			{
				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// spawn the projectile at the muzzle
				APlayersProjectile* tempP = World->SpawnActor<APlayersProjectile>(whiteProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
				if (tempP) {
					if (GEngine)
						GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, TEXT("SPAWNED"));
				}
				else {
					if (GEngine)
						GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, TEXT("ERROR"));
				}
			}
		}
	}else
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Yellow, TEXT("ERROR"));


	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void AMyPuzzleGameCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AMyPuzzleGameCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AMyPuzzleGameCharacter::UseGravityGun()
{
	if (!GravityGunOn) {
		GravityGunOn = true;
		muzzleLocation = FP_MuzzleLocation->GetComponentLocation();
		muzzleForwardVector = FP_MuzzleLocation->GetRightVector();
		endPoint = muzzleLocation + (muzzleForwardVector*distance);
		GetWorld()->LineTraceSingleByChannel(Hit, muzzleLocation, endPoint, ECollisionChannel::ECC_Visibility, FCollisionQueryParams::DefaultQueryParam);
		UKismetSystemLibrary::DoesImplementInterface(reflectorActor, IPickupable::UClassType::StaticClass());
		reflectorActor = dynamic_cast<AProjectileReflector*>(Hit.GetActor());		
		pickUpBeam->SetColorParameter("BaseColor", FLinearColor::FLinearColor(1,0,0));
	}
	else {
		GravityGunOn = false;
		if (reflectorActor) {
			reflectorActor->mainBody->SetSimulatePhysics(true);
			reflectorActor->mainBody->SetEnableGravity(true);
			reflectorActor->mainBody->SetNotifyRigidBodyCollision(false);
			reflectorActor->DetachFromActor(FDetachmentTransformRules::FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
			reflectorActor->floating = false;
			AMainReflector* tempReflector = Cast<AMainReflector>(reflectorActor->reflector->GetChildActor());
			if (tempReflector) {
				tempReflector->bIsFloating = false;
			}

			//delete tempReflector;
		}
		reflectorActor = NULL;
	}
}

void AMyPuzzleGameCharacter::RotateLeft()
{
	if (reflectorActor) {
		if (bRotateLeft)
			bRotateLeft = false;
		else
			bRotateLeft = true;
	}
}

void AMyPuzzleGameCharacter::RotateRight()
{
	if (reflectorActor) {
		if (bRotateRight)
			bRotateRight = false;
		else
			bRotateRight = true;
	}
}


void AMyPuzzleGameCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMyPuzzleGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

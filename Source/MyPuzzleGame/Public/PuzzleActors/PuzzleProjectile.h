// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleProjectile.generated.h"


UENUM()
enum EProjectilesColors {
	Red				UMETA(DisplayName = "BlueTarget"),
	Green			UMETA(DisplayName = "GreenTarget"),
	Blue			UMETA(DisplayName = "BlueTarget"),
	Yellow			UMETA(DisplayName = "YellowTarget")
};


UCLASS(Config=Game)
class MYPUZZLEGAME_API APuzzleProjectile : public AActor
{
	GENERATED_BODY()
	
		/** Sphere collision component */
		UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		class USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;

public:
	APuzzleProjectile();

	UFUNCTION()
		UProjectileMovementComponent* getProjectileMovement() { return ProjectileMovement; }

	/** called when projectile hits something */
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UPROPERTY(BlueprintReadWrite, Category=ProjectilesAttributes)
		FVector castOrigin;
	UPROPERTY(BlueprintReadWrite, Category=ProjectilesAttributes)
		FVector castDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		UStaticMeshComponent* sphereMesh;

	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MainAttributes)
		TEnumAsByte<EProjectilesColors> color;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MainAttributes)
		TArray<UMaterialInterface*> colorsArray;
};

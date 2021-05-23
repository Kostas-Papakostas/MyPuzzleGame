// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayersProjectile.generated.h"

UENUM()
enum EPlayersProjectileColor {
	Black			UMETA(DisplayName="Black"),
	White			UMETA(DisplayName="White")
};


UCLASS(Config=Game)
class MYPUZZLEGAME_API APlayersProjectile : public AActor
{
	GENERATED_BODY()

		/** Sphere collision component */
		UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		class USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;

public:
	APlayersProjectile();

	UFUNCTION()
		UProjectileMovementComponent* getProjectileMovement() { return ProjectileMovement; }

	/** called when projectile hits something */
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(BlueprintReadWrite, Category = ProjectilesAttributes)
		FVector castOrigin;
	UPROPERTY(BlueprintReadWrite, Category = ProjectilesAttributes)
		FVector castDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		UStaticMeshComponent* sphereMesh;

	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MainAttributes)
		TEnumAsByte<EPlayersProjectileColor> color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MainAttributes)
		TArray<UMaterialInterface*> colorsArray;

	
	
};

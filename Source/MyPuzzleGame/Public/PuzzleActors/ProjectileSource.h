// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleProjectile.h"
#include "ProjectileSource.generated.h"

UCLASS()
class MYPUZZLEGAME_API AProjectileSource : public AActor
{
	GENERATED_BODY()

		UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* mainBody;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* spawnPointMesh;

	UPROPERTY(EditDefaultsOnly)
		UMaterialInterface* bodyMaterial;



	UPROPERTY(EditDefaultsOnly)
		TArray<UMaterialInterface*> spawnEdgeMaterials;

	UPROPERTY(EditDefaultsOnly)
		USceneComponent* spawnPoint;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class APuzzleProjectile> ProjectileClass;

	FTimerHandle timerShootingProjectile;

public:	
	// Sets default values for this actor's properties
	AProjectileSource();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "Basic Attribute"))
		TEnumAsByte<EProjectilesColors> spawnMaterial;

	void SpawnBall();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};

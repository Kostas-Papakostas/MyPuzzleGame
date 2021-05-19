// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzleProjectile.h"
#include "GameFramework/Actor.h"
#include "PuzzleProjectileTarget.generated.h"

UCLASS(Config=Game)
class MYPUZZLEGAME_API APuzzleProjectileTarget : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
		bool noMoreKeys = false;

public:	
	// Sets default values for this actor's properties
	APuzzleProjectileTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "Basic Attribute"))
		TEnumAsByte<EProjectilesColors> color;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Category = "Mesh"))
		UStaticMeshComponent* outerTargetMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Category = "Mesh"))
		UStaticMeshComponent* innerTargetMesh;
	
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};

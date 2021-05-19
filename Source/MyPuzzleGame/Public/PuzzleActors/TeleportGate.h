// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TeleportGate.generated.h"

UCLASS()
class MYPUZZLEGAME_API ATeleportGate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeleportGate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Sphere collision component */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Teleporter)
		class UBoxComponent* overlapComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh)
		UStaticMeshComponent* vortexMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh)
		UStaticMeshComponent* frameMesh;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(BlueprintReadOnly, Category = DebugFields)
		bool teleported = false;
};

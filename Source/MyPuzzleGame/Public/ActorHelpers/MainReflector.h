// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainReflector.generated.h"

UCLASS()
class MYPUZZLEGAME_API AMainReflector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMainReflector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "Mesh Components Material"))
		UMaterialInterface* mainMeshMat;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "Mesh Components Material"))
		UMaterialInterface* mainMeshFloatingMat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "Mesh Components"))
		UStaticMeshComponent* mainMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "Component Locks"))
		bool bIsFloating;

	
};

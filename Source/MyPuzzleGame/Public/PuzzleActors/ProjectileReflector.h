// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileReflector.generated.h"

UCLASS()
class MYPUZZLEGAME_API AProjectileReflector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileReflector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "Mesh Components"))
		UStaticMeshComponent* mainBody;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "Mesh Components"))
		UStaticMeshComponent* bounceArea;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "Mesh Components"))
		UMaterial* bounceAreaMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "Mesh Components"))
		UMaterial* mainBodyMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "Child Actor"))
		UChildActorComponent* reflector;

	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleProjectile.h"
#include "Interfaces/Pickupable.h"
#include "ProjectileReflector.generated.h"

UCLASS()
class MYPUZZLEGAME_API AProjectileReflector : public AActor, public IPickupable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileReflector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface")
	void gyroscopicRotation();
	virtual void gyroscopicRotation_Implementation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "Lock Points"))
		bool floating;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Category = "Object Location"))
		FVector initialLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Category = "Object Location"))
		FRotator currentRotation;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
		void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "Mesh Components"))
		UStaticMeshComponent* mainBody;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "Mesh Components"))
		UStaticMeshComponent* bounceArea;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "Material Components"))
		UMaterialInterface* bounceAreaMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "Material Components"))
		UMaterialInterface* mainBodyMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "Material Components"))
		UMaterialInterface* bounceAreaFloatingMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "Material Components"))
		UMaterialInterface* mainBodyFloatingMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "Child Actor"))
		UChildActorComponent* reflector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "Overall Component"))
		class UBoxComponent* overallBox;
	
};

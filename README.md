# MyPuzzleGame

Whole project files:

https://drive.google.com/file/d/1SqxUleSNetJqv9eE-W-fKP_N9RwfVUso/view?usp=sharing

The project uses both blueprints and C++ code, the blueprints parts cannot be displayed here so they are accessible through the link above.

Game, playable on web:

https://cs.uoi.gr/~kpapakostas/

Gameplay video:

https://www.youtube.com/watch?v=HgR6-_W3ZL8

# Player Character related code:</br>

Basic movement and character functionality included in the PlayerActors/MyPuzzleGameCharacter.cpp

The pick up mechanic of the gun described here by disabling all physics of the actor and changing the colors of it as shown in the gameplay video.

```

void AMyPuzzleGameCharacter::Tick(float DeltaTime){

  if (GravityGunOn) {/*indicates if we pressed the action button */

      if (reflectorActor) {
        IPickupable* pickableActor = dynamic_cast<IPickupable*>(reflectorActor);
        if (pickableActor) {/*if the object inherits from IPickupable is able to be picked up*/
          /*if object got picked up any physics stuff
          got disabled to be attached and moved around*/
          reflectorActor->mainBody->SetSimulatePhysics(false);
          reflectorActor->mainBody->SetEnableGravity(false);
          /*attach to custom location*/
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
}
```

Pick up object mechanic code:</br>

```
void AMyPuzzleGameCharacter::UseGravityGun()
{
	if (!GravityGunOn) {/*check if we don't use the pick up action*/
		GravityGunOn = true;
		/*creates a line from a strating point towards its forward vector by distance units*/
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
		if (reflectorActor) {/*if pick up action is in use deactivates it*/
			...
			reflectorActor->DetachFromActor(FDetachmentTransformRules::FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
			reflectorActor->floating = false;
			AMainReflector* tempReflector = Cast<AMainReflector>(reflectorActor->reflector->GetChildActor());
			if (tempReflector) {
				tempReflector->bIsFloating = false;
			}

		}
		reflectorActor = NULL;
	}
}
```

Basic projectile the player can shoot:</br>

```
/*THE PROJECTILE A PLAYER SHOOT*/
// Sets default values
APlayersProjectile::APlayersProjectile()
{

	PrimaryActorTick.bCanEverTick = true;

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &APlayersProjectile::OnHit);	// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	...

	sphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	sphereMesh->SetRelativeScale3D(FVector(0.1, 0.1, 0.1));
	sphereMesh->SetupAttachment(RootComponent);

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void APlayersProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UWorld* const World = GetWorld();
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL)) {
		APuzzleBlocker* tempBlocker = Cast<APuzzleBlocker>(OtherActor);

		if (tempBlocker && tempBlocker->blockerColor==color) {//if projectile hits none of them then destroy
			OtherActor->Destroy();
			// try and play the sound if specified
			if (destroySound != NULL)
			{
				UGameplayStatics::PlaySoundAtLocation(this, destroySound, UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation());
			}
			this->Destroy();
		}
		else {
			this->Destroy();
		}
	}
}
```

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

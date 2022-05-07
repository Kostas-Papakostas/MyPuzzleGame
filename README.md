# MyPuzzleGame

Whole project files:

https://drive.google.com/file/d/1SqxUleSNetJqv9eE-W-fKP_N9RwfVUso/view?usp=sharing

The project uses both blueprints and C++ code, the blueprints parts cannot be displayed here so they are accessible through the link above.

Game, playable on web:

https://cs.uoi.gr/~kpapakostas/

Gameplay video:
Goal: Unlock the door to the next stage by unlocking all the locks at the same time, to achieve this player has to lead the balls at the correct locks. If any lock won't get a hit from the correct ball for 5seconds it locks again.
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
		//checks if the projectile hit any of the blockers capable to break
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

# Locks, projectiles and projectile sources:</br>

[Puzzle Directory](https://github.com/Kostas-Papakostas/MyPuzzleGame/tree/master/Source/MyPuzzleGame/Public/PuzzleActors)</br>

Projectile source is a simple mesh with a spawn point for the projectile:</br>

```
// Sets default values
AProjectileSource::AProjectileSource()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mainBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Main Body Comp"));
	mainBody->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	...

	spawnPointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Spoint Point Comp"));
	spawnPointMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	....

	spawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("CarryObjectLocation"));
	spawnPoint->SetupAttachment(spawnPointMesh);
	spawnPoint->SetRelativeLocation(FVector(0.2f, 230.0f, 32.0f));
}

// Called when the game starts or when spawned
void AProjectileSource::BeginPlay()
{
	Super::BeginPlay();
	if (spawnEdgeMaterials.Num() > 0) {
		mainBody->SetMaterial(0, bodyMaterial);
		spawnPointMesh->SetMaterial(0, spawnEdgeMaterials.operator[](spawnMaterial));
	}
	/*sets a timer to spawn a projectile every x seconds*/
	GetWorld()->GetTimerManager().SetTimer(timerShootingProjectile, this, &AProjectileSource::SpawnBall, 2.f, true);
}
```
Projectile class exists under the PuzzleActors folder and it's a simple projectile object with color variable that defines the target lock and texture of the projectile.</br>

# Main puzzle tools:</br>

Projectile Reflectors are used to reflect the projectile and can be moved by player. The projectile reflectors are using simple collisions to reflect projectile's way.
[Projectile Reflector](https://github.com/Kostas-Papakostas/MyPuzzleGame/blob/master/Source/MyPuzzleGame/Private/PuzzleActors/ProjectileReflector.cpp)</br>
[Projectile Reflector Header](https://github.com/Kostas-Papakostas/MyPuzzleGame/blob/master/Source/MyPuzzleGame/Public/PuzzleActors/ProjectileReflector.h)

```
AProjectileReflector::AProjectileReflector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	floating = false;

	....
	....
	
	//this part of the object reflects the projectiles
	bounceArea = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bounce Area"));
	bounceArea->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	bounceArea->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	bounceArea->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	
	reflector = CreateDefaultSubobject<UChildActorComponent>(TEXT("Reflector Area"));

	bounceArea->SetupAttachment(RootComponent);
	reflector->SetupAttachment(RootComponent);
	overallBox->OnComponentBeginOverlap.AddDynamic(this, &AProjectileReflector::OnOverlap);
	overallBox->OnComponentEndOverlap.AddDynamic(this, &AProjectileReflector::EndOverlap);
}
```

Projectile teleport gates:</br>
[Teleport Exit](https://github.com/Kostas-Papakostas/MyPuzzleGame/blob/master/Source/MyPuzzleGame/Private/PuzzleActors/TeleportExit.cpp)</br>
[Teleport Gate](https://github.com/Kostas-Papakostas/MyPuzzleGame/blob/master/Source/MyPuzzleGame/Private/PuzzleActors/TeleportGate.cpp)

```
void ATeleportGate::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TArray<AActor*> exitGates;
	UWorld* const World = GetWorld();
	UGameplayStatics::GetAllActorsOfClass(World, ATeleportExit::StaticClass(), exitGates);
	APuzzleProjectile* overlappedProjectile = dynamic_cast<APuzzleProjectile*>(OtherActor);
	FVector exitForwardVector;

	if (overlappedProjectile) {
		/*if the overlap object is a projectile then teleport to the corresponding gate*/
		for(AActor* myGate:exitGates){
			if (Cast<ATeleportExit>(myGate)->id == gateID) {
				exitForwardVector = myGate->GetActorRightVector();
				float tpHeight = this->GetActorLocation().Z - overlappedProjectile->GetActorLocation().Z;
				overlappedProjectile->GetProjectileMovement()->Velocity = exitForwardVector.GetSafeNormal()*overlappedProjectile->GetProjectileMovement()->MaxSpeed;
				//teleported = overlappedProjectile->TeleportTo(myGate->GetActorLocation(), overlappedProjectile->GetActorRotation());
				teleported = overlappedProjectile->TeleportTo(FVector::FVector(myGate->GetActorLocation().X, myGate->GetActorLocation().Y, myGate->GetActorLocation().Z-tpHeight), overlappedProjectile->GetActorRotation());
			}
		}
	}
}
```

# Main gate:

```
//The main functionality is to block the exit, every two seconds the gate checks if all the locks of the level are unlocked.

void AMainGate::checkIfUnlocked() {

	for (AActor* myActor : targetActors) {
		APuzzleProjectileTarget* myTarget = Cast<APuzzleProjectileTarget>(myActor);
		keysGathered += myTarget->keys;
	}

	if (keysGathered >= totalKeysToUnlock) {
		// try and play the sound if specified
		if (unlockSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, unlockSound, UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation());
		}
		doorUnLocked = true;
	}
	else
		keysGathered = 0;

	if (doorUnLocked) {
		/*if door gets unlocked destroy the main mesh */
		ACharacter* myCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		AMyPuzzleGameCharacter* puzzleCharacter = Cast<AMyPuzzleGameCharacter>(myCharacter);
		if (puzzleCharacter)
			puzzleCharacter->changeMaterial();
		mainFrame->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		if(mainGate)
			mainGate->DestroyComponent();
	}
}

```

// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerAvatar.h"
#include "Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	_LifeCountingDown = LifeSpan;
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(_LifeCountingDown > 0.0f) _LifeCountingDown -= DeltaTime;
	else {
		PrimaryActorTick.bCanEverTick = false;
		Destroy();
	}

	FVector currentLocation = GetActorLocation();
	FVector vel = GetActorRotation().RotateVector(FVector::ForwardVector) * Speed * DeltaTime;
	FVector nextLocation = currentLocation + vel;
	SetActorLocation(nextLocation);


	FHitResult hitResult;
	FCollisionObjectQueryParams objCollisionQueryParams;
	objCollisionQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

	if (GetWorld()->LineTraceSingleByObjectType(hitResult, currentLocation, nextLocation, objCollisionQueryParams)) {
		auto playerAvatar = Cast<APlayerAvatar>(hitResult.GetActor());

		if (playerAvatar != nullptr) {
			playerAvatar->Hit(Damage);
			PrimaryActorTick.bCanEverTick = false;
			Destroy();
		}
	}
}


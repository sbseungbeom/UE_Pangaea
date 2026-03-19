// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"
#include "PlayerAvatar.h"
#include "DefenseTower.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(_StaticMesh);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &AWeapon::OnWeaponBeginoverlap);
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Holder == nullptr) {
		FQuat rotQuat = FQuat(FRotator(0, 300.0f * DeltaTime, 0));
		AddActorLocalRotation(rotQuat);
	}


}

void AWeapon::OnWeaponBeginoverlap(AActor* OverlappedActor, AActor* OtherActor) {
	auto character = Cast<ACharacter>(OtherActor);

	if (character == nullptr) return;
	if (Holder == nullptr) {
		auto playerAvatar = Cast<APlayerAvatar>(character);
		if (playerAvatar != nullptr) {
			Holder = character;	//Holder는 무기를 집는 액터를 의미.
			TArray<AActor*> attachedActors;
			OtherActor->GetAttachedActors(attachedActors, true);	//attachedActors 배열에 OtherActor에 attach되어있는 액터들을 반환한다.

			for (int i = 0; i < attachedActors.Num(); ++i) {
				attachedActors[i]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);	//그 후 플레이어가 장착하고있던(Attach되어있던)것들을 Detach 시킨다.
				attachedActors[i]->SetActorRotation(FQuat::Identity);
				AWeapon* weapon = Cast<AWeapon>(attachedActors[i]);	//만약 Detach된게 무기라면,그 무기의 사용자를 nullptr로 한다.
				weapon->Holder = nullptr;							//다른 장비를 떨어뜨리게 할 수 있지만, 장비가 무기밖에 없기때문에 이렇게 작업한다.
			}
			AttachToComponent(Holder->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("hand_rSocket"));
		}
		else if (IsWithinAttackRange(0.0f, OtherActor)) {
			//deal damage to the Target : hero or enemy
		}
	}
}
bool AWeapon::IsWithinAttackRange(float AttackRange, AActor* Target) {
	//~~
	return true;
}

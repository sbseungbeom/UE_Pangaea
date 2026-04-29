// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PangaeaCharacterParent.h"
#include "Pangaea/Animation/PangaeaAnimInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Materials/Material.h"
#include "Engine/World.h"

// Sets default values
APangaeaCharacterParent::APangaeaCharacterParent()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APangaeaCharacterParent::BeginPlay()
{
	Super::BeginPlay();
	_AnimInstance = Cast<UPangaeaAnimInstance>(GetMesh()->GetAnimInstance());
	_HealthPoints = HealthPoints;
}

// Called every frame
void APangaeaCharacterParent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_AttackCountingDown > 0.0f) _AttackCountingDown -= DeltaTime;
}

int APangaeaCharacterParent::GetHealthPoints() {
	return _HealthPoints;
}
bool APangaeaCharacterParent::IsKilled() {
	return (_HealthPoints <= 0.0f);
}
bool APangaeaCharacterParent::CanAttack() {
	return (_AttackCountingDown <= 0.0f && _AnimInstance->State == ECharacterState::Locomotion);
}
bool APangaeaCharacterParent::IsAttacking() {
	return (_AnimInstance->State == ECharacterState::Attack);
}
void APangaeaCharacterParent::Attack() {
	_AttackCountingDown = Attackinterval;
	if (!IsAttacking()) {
		_AnimInstance->State = ECharacterState::Attack;
	}

}
void APangaeaCharacterParent::Hit(int Damage) {
	_HealthPoints -= Damage;
	_AnimInstance->State = ECharacterState::Hit;
	if (IsKilled()) {
		DieProcess();
	}
}
void APangaeaCharacterParent::DieProcess() {
	Destroy();
}




// Called to bind functionality to input
void APangaeaCharacterParent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APangaeaCharacterParent::Attack_Broadcast_RPC_Implementation() {
	Attack();
}


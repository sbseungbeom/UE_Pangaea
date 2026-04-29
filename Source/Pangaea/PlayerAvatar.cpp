// Fill out your copyright notice in the Description page of Project Settings.

#include "Perception/AISenseConfig_Sight.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/PangaeaAnimInstance.h"
#include "PlayerAvatar.h"

// Sets default values
APlayerAvatar::APlayerAvatar()
{
	
	auto characterMovement = GetCharacterMovement();
	characterMovement->bOrientRotationToMovement = true;
	characterMovement->RotationRate = FRotator(0.0f, 640.0f, 0.0f);
	characterMovement->bConstrainToPlane = true;
	characterMovement->bSnapToPlaneAtStart = true;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//Set up Spring Arm Component
	_springArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	_springArmComponent->SetupAttachment(RootComponent);
	_springArmComponent->SetUsingAbsoluteRotation(true);
	_springArmComponent->TargetArmLength = 800.0f;
	_springArmComponent->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));
	_springArmComponent->bDoCollisionTest = false;

	//Set up Camera Component
	_cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	_cameraComponent->SetupAttachment(_springArmComponent, USpringArmComponent::SocketName);
	_cameraComponent->bUsePawnControlRotation = false;

	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;



}
void APlayerAvatar::BeginPlay()
{
	Super::BeginPlay();

}
void APlayerAvatar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	_AnimInstance->Speed = GetCharacterMovement()->Velocity.Size2D();
	//여기서 획득한 velocity 값은 월드 좌표 시스템의 x,y,z 축의 속도를 가리키는 FVector 이므로,
	//우리가 필요한것은 x,y 이니 FVector의 Size2D()의 값으로 이를 반환한다.
}

// Called to bind functionality to input
void APlayerAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerAvatar::AttachWeapon(AWeapon* weapon) {
	weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("hand_rSocket"));
}
void APlayerAvatar::DropWeapon() {

	TArray<AActor*> attachedActors;
	GetAttachedActors(attachedActors, true);
	for (int i = 0; i < attachedActors.Num(); ++i) {
		attachedActors[i]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		attachedActors[i]->SetActorRotation(FQuat::Identity);
		AWeapon* weapon = Cast<AWeapon>(attachedActors[i]);
		if (weapon != nullptr) {
			weapon->Holder = nullptr;
		}
	}
}
void APlayerAvatar::Attack() {
	APangaeaCharacterParent::Attack();
}

void APlayerAvatar::Attack_RPC_Implementation() {
	Attack_Broadcast_RPC();
}


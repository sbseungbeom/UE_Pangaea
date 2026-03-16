// Fill out your copyright notice in the Description page of Project Settings.

#include "Perception/AISenseConfig_Sight.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerAvatarAnimInstance.h"
#include "PlayerAvatar.h"

// Sets default values
APlayerAvatar::APlayerAvatar()
{
	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));
	
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


// Called when the game starts or when spawned
void APlayerAvatar::BeginPlay()
{
	Super::BeginPlay();
	if (StimuliSource) {
		StimuliSource->RegisterForSense(UAISense_Sight::StaticClass());
		StimuliSource->RegisterWithPerceptionSystem();
	}
	
}

// Called every frame
void APlayerAvatar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UPlayerAvatarAnimInstance* animInst = Cast<UPlayerAvatarAnimInstance>(GetMesh()->GetAnimInstance());
	if (animInst) {
		animInst->Speed = GetCharacterMovement()->Velocity.Size2D(); 
		//여기서 획득한 velocity 값은 월드 좌표 시스템의 x,y,z 축의 속도를 가리키는 FVector 이므로,
		// 우리가 필요한것은 x,y 이니 FVector의 Size2D()의 값으로 이를 반환한다.
	}

	if(_AttackCountingDown > 0.0f) _AttackCountingDown -= DeltaTime;
	
}

// Called to bind functionality to input
void APlayerAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


}

int APlayerAvatar::GetHealthPoints()
{
	return _HealthPoints;
}

bool APlayerAvatar::IsKilled()
{
	return (_HealthPoints <= 0.0f);
}

bool APlayerAvatar::CanAttack()
{
	UPlayerAvatarAnimInstance* animInst = Cast<UPlayerAvatarAnimInstance>(GetMesh()->GetAnimInstance());
	return (_AttackCountingDown <= 0.0f && animInst->State == EPlayerState::Locomotion); //공격애니메이션이 끝났고, 공격 쿨타임이 끝났을 때 공격 가능.
}
void APlayerAvatar::Attack() {
	_AttackCountingDown = AttackInterval;
	UPlayerAvatarAnimInstance* animInst = Cast<UPlayerAvatarAnimInstance>(GetMesh()->GetAnimInstance());
	animInst->State = EPlayerState::Attack;
}
void APlayerAvatar::Hit(int Damage) {
	UE_LOG(LogTemp, Warning, TEXT("Hit"));
}

void APlayerAvatar::DieProcess() {
	Destroy();
	/*
	*이 작업은 Destroy() 단 한줄로 치환 가능하다.
	PrimaryActorTick.bCanEverTick = false;
	Destroy();
	GEngine->ForceGarbageCollection(true);
	*/
	
}


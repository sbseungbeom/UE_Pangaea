
#include "PlayerAvatar.h"
#include "Weapons/Weapon.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy/EnemyAIController.h"
#include "Enemy/EnemyAnimInstance.h"
#include "Enemy/Enemy.h"

// Sets default values
AEnemy::AEnemy()
{
	SetCanAffectNavigationGeneration(true);

	PrimaryActorTick.bCanEverTick = true;
	

	//ConstructorHelprs::FObjectFinder 구조체는 프로젝트의 특정 경로에서 에셋을 찾는걸 도와준다.
	//꺾쇠 괄호 안에 UBlueprint를 템플릿 클래스로 입력해뒀는데, 이는 우리가 찾으려는 에셋이 블루프린트임을 알려준다.
	//변수의 이름은 blueprint_finder 이다.
	static ConstructorHelpers::FObjectFinder<UBlueprint> blueprint_finder(TEXT("/ Script / Engine.Blueprint'/Game/BluePrints/Weapons/BP_Hammer.BP_Hammer'"));

	//blueprint_finder.Object->GeneratedClass의 반환값은 발견된 에셋의 UClass 값이며, 이 값이 _WeaponClass에 할당된다.
	_WeaponClass = (UClass*)blueprint_finder.Object->GeneratedClass;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	_HealthPoints = HealthPoints;

	AnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	EnemyController = Cast<AEnemyAIController>(GetController());

	_Weapon = Cast<AWeapon>(GetWorld()->SpawnActor(_WeaponClass));
	_Weapon->Holder = this;
	_Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("hand_rSocket"));

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (AnimInstance == nullptr || EnemyController == nullptr) {
		AnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
		EnemyController = Cast<AEnemyAIController>(GetController());
	}

	AnimInstance->Speed = GetCharacterMovement()->Velocity.Size2D();
	//1. 엔진의 이동 컴포넌트에서 속도값을 가져온다. (GetCharacterMovement()->Velocity)
	//2. 이 속도값은 월드 좌표 시스템의 x,y,z 축의 속도를 가리키는 FVector 이므로, 바닥에서 이동하는 순수한 수평 속도값을 위해 Size2D()의 값으로 이를 반환한다.
	
	if (_AttackCountingDown > 0.0f) _AttackCountingDown -= DeltaTime;

	//타겟이 존재하고 , 현재 애니메이션이 Locomotion 상태라면 EnemyAIController의 MakeAttackDecision 함수를 호출한다.
	//MakeAttackDecision 함수는 타겟과의 거리를 계산하여 공격 범위 내에 있다면 공격을 수행한다.
	if (_chasedTarget != nullptr && AnimInstance->State == EEnemyState::Locomotion) {
		EnemyController->MakeAttackDecision(_chasedTarget);
	}
}

int AEnemy::GetHealthPoints() {return _HealthPoints;}
bool AEnemy::IsKilled() { return _HealthPoints <= 0.0f; }
bool AEnemy::CanAttack() { return(_AttackCountingDown <= 0.0f && AnimInstance->State == EEnemyState::Locomotion); }
void AEnemy::Attack() {
	EnemyController->StopMovement();
	EnemyController->bIsChasing = false;
	_AttackCountingDown = AttackInterval;
	AnimInstance->State = EEnemyState::Attack;
}
void AEnemy::Hit(int damage) {
	_HealthPoints -= damage;
	AnimInstance->State = EEnemyState::Hit;

	if (IsKilled()) { DieProcess();}
}
void AEnemy::DieProcess() {
	Destroy();
}
void AEnemy::Set_ChasedTarget(APawn* Target) {
	_chasedTarget = Target;
}






#include "PlayerAvatar.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy/EnemyAIController.h"
#include "Enemy/EnemyAnimInstance.h"
#include "Enemy/Enemy.h"

// Sets default values
AEnemy::AEnemy()
{
	SetCanAffectNavigationGeneration(true);

	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	_HealthPoints = HealthPoints;

	AnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	EnemyController = Cast<AEnemyAIController>(GetController());

	//감지했을 때 실행될 함수(델리게이트) 연결.
	//if (AI_Sensor) AI_Sensor->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemy::OnTargetDetected);

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





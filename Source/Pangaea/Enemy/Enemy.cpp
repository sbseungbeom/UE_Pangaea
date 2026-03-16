
#include "PlayerAvatar.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"	//AIPerception 컴포넌트를 사용하기 위해, 시야 센서의 헤더를 가져옴.
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy/EnemyAIController.h"
#include "Enemy/EnemyAnimInstance.h"
#include "Enemy/Enemy.h"

// Sets default values
AEnemy::AEnemy()
{
	SetCanAffectNavigationGeneration(true);

	PrimaryActorTick.bCanEverTick = true;
	/*
	//컴포넌트 설정
	AI_Sensor = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Pawn Sensor"));
	AI_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AI Sight"));

	//센서(AI_Sight) 를 레이더(AI_Sensor) 에 등록
	AI_Sensor->ConfigureSense(*AI_Sight);	
	AI_Sensor->SetDominantSense(AI_Sight->GetSenseImplementation());

	//시야 세부 설정
	AI_Sight->SightRadius = 1500.0f;		//인지 거리
	AI_Sight->LoseSightRadius = 2000.0f;	//놓치는 거리
	AI_Sight->PeripheralVisionAngleDegrees = 45.0f; //시야각
	AI_Sight->DetectionByAffiliation.bDetectEnemies = true;	
	AI_Sight->DetectionByAffiliation.bDetectNeutrals = true;
	AI_Sight->DetectionByAffiliation.bDetectFriendlies = true;
	/*
		언리얼엔진의 팀 시스템에 따라 감지대상을 선택함.
		하지만 Actor는 태생적으로 팀이 없다. 즉, no team상태다.
		언리얼 팀 시스템을 따로 구현한게 아니라면 엔진은 모든 액터를 neutral(중립)으로 구분한다.
		
		만약 DetectionByAffililation.bDetectEnemies 만 true로 두었다면?
		AI는 플레이어를 봐도 "어? 저건 Enemy가 아니라 Neutral 이구나!" 라고 하면서 무시할수도 있다.

		일단, 세 개를 다 true 로 두면, AI의 논리는 다음과 같다.
		"네가 우리팀이든, 적팀이든, 중립이든 일단 '누군가' 보이기만 하면 감지하겠다!"
		즉, 복잡한 팀 설정을 건너뛰고 "시야에 들어온 모든 액터" 를 감지할 수 있게 된다.
	*/
	
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

/*
void AEnemy::Chase(APawn* targetPawn) {
	if (targetPawn != nullptr && AnimInstance->State == EEnemyState::Locomotion) {
		EnemyController->MoveToActor(targetPawn, 90.0f);
	}
	_chasedTarget = targetPawn;
}
*/

/*
void AEnemy::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)  {
	// 1. AnimInstance가 유효한지 먼저 확인 (가장 중요)
	if (AnimInstance == nullptr) {
		AnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
		if (AnimInstance == nullptr) return; // 여전히 null이면 실행 중단
	}
	// 2. 조건문 검사
	if (Stimulus.WasSuccessfullySensed() && Actor != nullptr && Actor->IsA(APlayerAvatar::StaticClass()))
	{
		// 상태 체크를 분리하거나 안전하게 수행
		if (AnimInstance->State == EEnemyState::Locomotion)
		{
			_chasedTarget = Cast<APlayerAvatar>(Actor);

			// EnemyController도 안전하게 체크
			if (EnemyController == nullptr) {
				EnemyController = Cast<AEnemyAIController>(GetController());
			}

			if (EnemyController != nullptr) {
				EnemyController->MoveToActor(_chasedTarget, 90.0f);
			}
		}
	}
	else
	{
		_chasedTarget = nullptr;
		// 추격 중지 로직이 필요할 수 있습니다 (예: EnemyController->StopMovement())
		UE_LOG(LogTemp, Display, TEXT("놓쳤다..."));
	}
}
*/



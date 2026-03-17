// Fill out your copyright notice in the Description page of Project Settings.




#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"	//AIPerception 컴포넌트를 사용하기 위해, 시야 센서의 헤더를 가져옴.
#include "Enemy/Enemy.h"
#include "Enemy/EnemyAIController.h"

//생성자로 초기 값 설정
AEnemyAIController::AEnemyAIController() {

	AI_Sensor = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception"));
	AI_SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AI Sight Config"));

	AI_SightConfig->SightRadius = 1500.0f;
	AI_SightConfig->LoseSightRadius = 2000.0f;
	AI_SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	AI_SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	AI_SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	AI_SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AI_Sensor->ConfigureSense(*AI_SightConfig);
	AI_Sensor->SetDominantSense(AI_SightConfig->GetSenseImplementation());

}

void AEnemyAIController::BeginPlay() {
	Super::BeginPlay();

	Enemy = Cast<AEnemy>(this->GetPawn());
	AI_Sensor->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetDetected);

}


void AEnemyAIController::MakeAttackDecision(APawn* TargetPawn) {

	AEnemy* ControlledCharacter = Cast<AEnemy>(GetPawn());
	auto dist = FVector::Dist2D(TargetPawn->GetActorLocation(),ControlledCharacter->GetActorLocation());
	
	if (dist <= ControlledCharacter->AttackRange && ControlledCharacter->CanAttack()) {
		ControlledCharacter->Attack();
	}
}

void AEnemyAIController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus) {
	
	if (Enemy == nullptr) {
		Enemy = Cast<AEnemy>(GetPawn());
	}

	// 2. 조건문 검사
	if (Stimulus.WasSuccessfullySensed() && Actor != nullptr)
	{
		//3.컨트롤러 이동 명령
		UE_LOG(LogTemp, Display, TEXT("쫓아간다!!"));
		this->MoveToActor(Actor, 90.0f);
	
		APawn* TargetPawn = Cast<APawn>(Actor);
		if (TargetPawn && Enemy) {
			Enemy->Set_ChasedTarget(TargetPawn);
		}
	}
	else
	{
		// 추격 중지 로직이 필요할 수 있습니다 (예: this->StopMovement())
		UE_LOG(LogTemp, Display, TEXT("놓쳤다..."));
		this->StopMovement();

		if (Enemy) {
			Enemy->Set_ChasedTarget(nullptr);
		}
	}
}


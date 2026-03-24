// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class PANGAEA_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();
	
	UPROPERTY(EditAnywhere, Category = "AI")
	bool bIsChasing;
	UPROPERTY(EditAnywhere, Category = "AI")
	APawn* TargetPawn;

	UFUNCTION()
	void MakeAttackDecision(APawn* _TargetPawn);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus);

private:
	UPROPERTY()
	class APlayerAvatar* Player;

	UPROPERTY()
	class AEnemy* Enemy;



	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UAISenseConfig_Sight* AI_SightConfig;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"), Category = "AI")
	class UAIPerceptionComponent* AI_Sensor;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	float ChaseRetentionTime; //플레이어를 시야에서 놓치고도 추적을 유지하는 시간
	UPROPERTY(VisibleAnywhere, Category = "AI")
	float ChasingTime;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	bool isChasingRetentionSucceed; //플레이어를 시야에서 놓치고도 추적을 성공했는지 확인하는 불리언
	UPROPERTY(VisibleAnywhere, Category = "AI")
	FVector InitialLocation;	//스폰 시 위치
	UFUNCTION(BlueprintCallable, Category = "AI")
	void KeepChasing();


	UFUNCTION(BlueprintCallable , Category = "AI")
	void ResetChasing();

	UPROPERTY()
	FTimerHandle ChasingTimerHandle;
};

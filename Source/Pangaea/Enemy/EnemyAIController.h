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
	

	UFUNCTION()
	void MakeAttackDecision(APawn* TargetPawn);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus);

private:
	UPROPERTY()
	class AEnemy* Enemy;



	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UAISenseConfig_Sight* AI_SightConfig;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"), Category = "AI")
	class UAIPerceptionComponent* AI_Sensor;
};

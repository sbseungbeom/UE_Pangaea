// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class PANGAEA_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();


	UPROPERTY(EditAnywhere, Category = "Enemy Params")
	int HealthPoints = 100;
	UPROPERTY(EditAnywhere, Category = "Enemy Params")
	float Strength = 5.0f;
	UPROPERTY(EditAnywhere, Category = "Enemy Params")
	float Armor = 1;
	UPROPERTY(EditAnywhere, Category = "Enemy Params")
	float AttackRange = 200.0f;
	UPROPERTY(EditAnywhere, Category = "Enemy Params")
	float AttackInterval = 3.0f;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	int _HealthPoints;
	UPROPERTY()
	float _AttackCountingDown;
	UPROPERTY()
	APawn* _chasedTarget = nullptr;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Pangaea | Enemy", meta = (DisplayName = "Get HP"))
	int GetHealthPoints();
	UFUNCTION(BlueprintCallable, Category = "Pangaea | Enemy")
	bool IsKilled();
	UFUNCTION(BlueprintCallable, Category = "Pangaea | Enemy")
	bool CanAttack();
	UFUNCTION(BlueprintCallable, Category = "Pangaea | Enemy")
	void Chase(APawn* targetPawn);

	UFUNCTION(BlueprintCallable, Category = "Pangaea | Enemy")
	void Attack();
	UFUNCTION(BlueprintCallable, Category = "Pangaea | Enemy")
	void Hit(int damage);
	UFUNCTION(BlueprintCallable, Category = "Pangaea | Enemy")
	void DieProcess();



private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Meta=(AllowPrivateAccess = "true"))
	class UAIPerceptionComponent* AI_Sensor;

	UPROPERTY()
	class UEnemyAnimInstance* AnimInstance;
	UPROPERTY()
	class AEnemyAIController* EnemyController;

};

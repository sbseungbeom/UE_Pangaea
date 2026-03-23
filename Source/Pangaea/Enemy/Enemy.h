// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Character/PangaeaCharacterParent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class PANGAEA_API AEnemy : public APangaeaCharacterParent
{
	GENERATED_BODY()

public:
	AEnemy();
protected:
	virtual void BeginPlay() override;


	UPROPERTY()
	APawn* _chasedTarget = nullptr;
	UClass* _WeaponClass;
	class AWeapon* _Weapon;
	
public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Pangaea | Enemy")
	void Set_ChasedTarget(APawn* Target);

	void Attack() override;
	void DieProcess() override;


private:

	UPROPERTY(VisibleAnywhere, Category = "AI")
	class AEnemyAIController* EnemyController;




};

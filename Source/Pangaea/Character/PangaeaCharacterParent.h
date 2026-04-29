// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PangaeaCharacterParent.generated.h"

UCLASS(Blueprintable)
class PANGAEA_API APangaeaCharacterParent : public ACharacter
{
	GENERATED_BODY()

public:
	APangaeaCharacterParent();

	UPROPERTY(EditAnywhere, Category = "Pangaea Character Params")
	int HealthPoints = 100;
	UPROPERTY(EditAnywhere, Category = "Pangaea Character Params")
	float Strength = 5;
	UPROPERTY(EditAnywhere, Category = "Pangaea Character Params")
	float Armor = 1;
	UPROPERTY(EditAnywhere, Category = "Pangaea Character Params")
	float AttackRange = 200.0f;
	UPROPERTY(EditAnywhere, Category = "Pangaea Character Params")
	float Attackinterval = 3.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class UPangaeaAnimInstance* _AnimInstance;

	UPROPERTY(Replicatedusing = OnHealthPointsChanged)
	int _HealthPoints;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	float _AttackCountingDown;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Pangaea|Character" ,meta = (DisplayName = "Get HP"))
	int GetHealthPoints();
	UFUNCTION(BlueprintCallable, Category = "Pangaea|Character")
	bool IsKilled();
	UFUNCTION(BlueprintCallable, Category = "Pangaea|Character")
	bool CanAttack();
	UFUNCTION(BlueprintCallable, Category = "Pangaea|Character")
	bool IsAttacking();

	virtual void Attack();
	virtual void Hit(int Damage);
	virtual void DieProcess();

	UFUNCTION(NetMulticast, Reliable)
	void Attack_Broadcast_RPC();
	UFUNCTION()
	void OnHealthPointsChanged();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UUserWidget* HealthBarWidget;

 
};

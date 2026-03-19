// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Perception/AIPerceptionStimuliSourceComponent.h" //ai가 감지하는 대상에 등록하기 위해 필요함
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PlayerAvatar.generated.h"

UCLASS(Blueprintable)
class PANGAEA_API APlayerAvatar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerAvatar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere, Category = "PlayerAvatar Params")
	int HealthPoints = 500;
	UPROPERTY(EditAnywhere, Category = "PlayerAvatar Params")
	float Strength = 10.0f;
	UPROPERTY(EditAnywhere, Category = "PlayerAvatar Params")
	float Armer = 3;
	UPROPERTY(EditAnywhere, Category = "PlayerAvatar Params")
	float AttackRange = 6.0f;
	UPROPERTY(EditAnywhere, Category = "PlayerAvatar Params")
	float AttackInterval = 1.2f;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UAIPerceptionStimuliSourceComponent* StimuliSource;

	UFUNCTION(BlueprintCallable, Category = "Pangaea | Player Character", Meta=(DisplayName="Get Hp"))
	int GetHealthPoints();
	UFUNCTION(BlueprintCallable, Category = "Pangaea | Player Character")
	bool IsKilled();
	UFUNCTION(BlueprintCallable, Category = "Pangaea | Player Character")
	bool CanAttack();
	UFUNCTION(BlueprintCallable, Category = "Pangaea | Player Character")
	void Attack();
	UFUNCTION(BlueprintCallable, Category = "Pangaea | Player Character")
	bool IsAttacking();
	UFUNCTION(BlueprintCallable, Category = "Pangaea | Player Character")
	void Hit(int Damage);
	UFUNCTION(BlueprintCallable, Category = "Pangaea | Player Character")
	void DieProcess();

	

protected:
	int _HealthPoints;
	float _AttackCountingDown;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* _springArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* _cameraComponent;

	FORCEINLINE USpringArmComponent* GetSpringArmComponent() const { return _springArmComponent; }
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return _cameraComponent; }
};

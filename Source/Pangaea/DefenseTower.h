// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DefenseTower.generated.h"

UCLASS(Blueprintable)
class PANGAEA_API ADefenseTower : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADefenseTower();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	UPROPERTY(EditAnywhere, Category = "Tower Params")
	int HealthPoints = 100;
	UPROPERTY(EditAnywhere, Category = "Tower Params")
	int ShellDefnse = 2;
	UPROPERTY(EditAnywhere, Category = "Tower Params")
	float AttackRange = 15.0f;
	UPROPERTY(EditAnywhere, Category = "Tower Params")
	float ReloadInterval = 1.0f;

	UFUNCTION(BlueprintCallable, Category = "Pangaea | Defense Tower", Meta = (DisplayName = "Get Hp"))
	int GetHealthPoints();
	UFUNCTION(BlueprintCallable, Category = "Pangaea | Defense Tower")
	bool IsDestroyed();
	UFUNCTION(BlueprintCallable, Category = "Pangaea | Defense Tower")
	bool CanFire();
	void Fire();
	void Hit(int Damage);

	FORCEINLINE UBoxComponent* GetBoxComponent() const { return _BoxComponent; }	//CONST를 붙여 읽기 전용 함수로 만든다. GETTER 함수는 보통 읽기 전용이므로 CONST를 붙이는 것이 일반적이다. 값을 수정하지 않기에.
	FORCEINLINE UStaticMeshComponent* GetMeshComponent() const { return _MeshComponent; }

protected:
	int _HealthPoints;
	float _ReloadCountingDown;

	void DestroyProcess();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower Component", Meta = (AllowPrivateAccess = "true"))
	UBoxComponent* _BoxComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower Component", Meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* _MeshComponent;


};

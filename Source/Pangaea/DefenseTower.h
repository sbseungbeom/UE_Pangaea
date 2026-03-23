// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
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

	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,	 //이벤트를 발생시키는 컴포넌트
		AActor* OtherActor,							// 영역에 들어오거나 나가는 액터의 포인터
		UPrimitiveComponent* OtherComponent,		// 다른 액터의 컴포넌트
		int32 OtherBodyIndex,						//다른 액터의 바디 인덱스
		bool bFromSweep, 
		const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverLap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex);
	UFUNCTION(BlueprintCallable , Category = "Pangaea | Defense Tower")
	void OnMeshBeginOverlap(AActor* OtherActor);


	UPROPERTY()
	class APlayerAvatar* _Target = nullptr;			//타겟은 플레이어 하나밖에 없어서 이렇게 저장.


	UFUNCTION()
	void Fire();
	UFUNCTION()
	void Hit(int Damage);

	FORCEINLINE USphereComponent* GetSphereComponent() const { return _SphereComponent; }	//CONST를 붙여 읽기 전용 함수로 만든다. GETTER 함수는 보통 읽기 전용이므로 CONST를 붙이는 것이 일반적이다. 값을 수정하지 않기에.
	FORCEINLINE UStaticMeshComponent* GetMeshComponent() const { return _MeshComponent; }


protected:

	class APangaeaGameMode* _PangaeaGameMode;

	int _HealthPoints;
	float _ReloadCountingDown;

	void DestroyProcess();

	UClass* _FireballClass;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower Component", Meta = (AllowPrivateAccess = "true"))
	USphereComponent* _SphereComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower Component", Meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* _MeshComponent;


};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Weapon.generated.h"

UCLASS()
class PANGAEA_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ACharacter* Holder = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Weapon Params")
	float Strength = 10;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* _StaticMesh;

	UFUNCTION()
	void OnWeaponBeginoverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	bool IsWithinAttackRange(float AttackRange, AActor* Target);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

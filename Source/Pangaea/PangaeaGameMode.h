// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Projectile.h"
#include "PangaeaGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PANGAEA_API APangaeaGameMode : public AGameMode
{
	GENERATED_BODY()


public:
	APangaeaGameMode();
	virtual ~APangaeaGameMode();

	AProjectile* SpawnOrGetFireball(UClass* ProjectileClass);
	void RecycleFireball(AProjectile* projectile);

	// 타워가 파괴됐을 때 Tower가 호출 → 모든 PC에 Victory UI 표시 RPC 송출
	void NotifyTowerDestroyed();

protected:
	TQueue<AProjectile*, EQueueMode::Spsc> _FireballPool; //EQueueMode::Spsc중 spsc는 single producer , single consumer 이다.
															//만드는 놈 한명, 쓰는놈 한명인 상황에서 가장 빠르게 작동하도록 설계되어 있다.

};

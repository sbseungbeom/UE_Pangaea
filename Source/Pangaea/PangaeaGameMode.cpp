// Fill out your copyright notice in the Description page of Project Settings.


#include "PangaeaGameMode.h"

APangaeaGameMode::APangaeaGameMode()
{
    /*
    // 1. 에셋 경로를 이용해 블루프린트 클래스를 찾습니다.
    // 주의: 블루프린트 클래스 경로는 뒤에 '_C'를 붙여야 정확하게 클래스 타입을 가져옵니다. (블루프린트 - 레퍼런스 복사해서 붙여넣기)
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/BluePrints/BP_PlayerAvatar.BP_PlayerAvatar_C"));

    if (PlayerPawnBPClass.Class != nullptr)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
    */

}

APangaeaGameMode::~APangaeaGameMode()   //~는 소멸자로, 이 클래스가 사라질 때 마지막으로 실행되는 함수이다.
{
    AProjectile* fireball;
    while (!_FireballPool.IsEmpty() && _FireballPool.Dequeue(fireball))
    {
        fireball->Destroy();
    }
    _FireballPool.Empty();
}


AProjectile* APangaeaGameMode::SpawnOrGetFireball(UClass* projectileClass) {
    AProjectile* fireball = nullptr;

    if (_FireballPool.IsEmpty()) {
        fireball = Cast<AProjectile>(GetWorld()->SpawnActor(projectileClass));
    }
    else {
        _FireballPool.Dequeue(fireball);
        fireball->Reset(); 
    }
    return fireball;
}

void APangaeaGameMode::RecycleFireball(AProjectile* projectile) {
    if (projectile == nullptr) return;

    projectile->SetActorHiddenInGame(true);
    projectile->SetActorEnableCollision(true);
    projectile->SetActorTickEnabled(false);
    _FireballPool.Enqueue(projectile);
}



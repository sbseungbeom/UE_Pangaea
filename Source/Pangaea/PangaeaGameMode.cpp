// Fill out your copyright notice in the Description page of Project Settings.


#include "PangaeaGameMode.h"

APangaeaGameMode::APangaeaGameMode()
{
    // 1. 에셋 경로를 이용해 블루프린트 클래스를 찾습니다.
    // 주의: 블루프린트 클래스 경로는 뒤에 '_C'를 붙여야 정확하게 클래스 타입을 가져옵니다. (블루프린트 - 레퍼런스 복사해서 붙여넣기)
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/BluePrints/BP_PlayerAvatar.BP_PlayerAvatar_C"));

    if (PlayerPawnBPClass.Class != nullptr)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
}
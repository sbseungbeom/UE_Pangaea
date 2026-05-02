// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerAvatar.h"
#include "Projectile.h"
#include "Pangaea/PangaeaCharacter.h"
#include "Weapons/Weapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PangaeaGameMode.h"
#include "DefenseTower.h"

// Sets default values
ADefenseTower::ADefenseTower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SetRootComponent(_SphereComponent);

	_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	_MeshComponent->SetupAttachment(_SphereComponent);


	_SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ADefenseTower::OnBeginOverlap);
	_SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ADefenseTower::OnEndOverLap);

	static ConstructorHelpers::FObjectFinder<UBlueprint> blueprint_finder(TEXT("/Script/Engine.Blueprint'/Game/BluePrints/DefenseTower/BP_Fireball.BP_Fireball'"));
	_FireballClass = (UClass*)blueprint_finder.Object->GeneratedClass;
}

void ADefenseTower::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,	 //이벤트를 발생시키는 컴포넌트
	AActor* OtherActor,							// 영역에 들어오거나 나가는 액터의 포인터
	UPrimitiveComponent* OtherComponent,		// 다른 액터의 컴포넌트
	int32 OtherBodyIndex,						//다른 액터의 바디 인덱스
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	APlayerAvatar* player = Cast<APlayerAvatar>(OtherActor);

	if (player) {
		_Target = player;
	}
}
void ADefenseTower::OnEndOverLap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex) {
	if (_Target != nullptr && OtherActor == _Target) {
		_Target = nullptr;
	}
}

// Called when the game starts or when spawned
void ADefenseTower::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickInterval(0.5f);
	_PangaeaGameMode = Cast<APangaeaGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

}

// Called every frame
void ADefenseTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_Target != nullptr && GetNetMode() != NM_Client) {
		Fire();
	}

	// 서버에서만 HP를 검사해서 0 이하가 되면 파괴 처리 1회 실행
	if (GetNetMode() != NM_Client && !_bDestroyed && HealthPoints <= 0) {
		DestroyProcess();
	}
}

void ADefenseTower::Fire() {
	//auto fireball = Cast<AProjectile>(GetWorld()->SpawnActor(_FireballClass));
	auto fireball = _PangaeaGameMode->SpawnOrGetFireball(_FireballClass);
	FVector startLocation = GetActorLocation();
	startLocation.Z += 100.0f;

	FVector targetLocation = _Target->GetActorLocation();
	targetLocation.Z = startLocation.Z;

	FRotator rotation = UKismetMathLibrary::FindLookAtRotation(startLocation, targetLocation);
	fireball->SetActorLocation(startLocation);
	fireball->SetActorRotation(rotation);
}
void ADefenseTower::OnMeshBeginOverlap(AActor* OtherActor) {
	// 데미지 판정은 서버 권한. 클라쪽 호출은 즉시 컷 → 양쪽 데미지 중복 방지
	if (GetNetMode() == NM_Client) return;

	AWeapon* weapon = Cast<AWeapon>(OtherActor);
	if (weapon == nullptr || weapon->Holder == nullptr) {
		return;
	}
	APlayerAvatar* player = Cast<APlayerAvatar>(weapon->Holder);
	if (player) {
		UE_LOG(LogTemp, Warning, TEXT("WEAPON->HOLDER 로 플레이어 캐스팅 성공."));
		if (player->IsAttacking() && CanBeDamaged()) {
			//Hit(weapon->Holder->Strength)여야 하는데, 책이 진짜 너무불친절하게 만들어져 있음.
			Hit(5);
			UE_LOG(LogTemp, Warning, TEXT("맞았다"));
		}

	}
	else UE_LOG(LogTemp, Warning, TEXT("캐스팅 실패"));
}


int ADefenseTower::GetHealthPoints()
{
	return _HealthPoints;
}

bool ADefenseTower::IsDestroyed()
{
	return (_HealthPoints > 0.0f);
}

void ADefenseTower::Hit(int Damage) {
	HealthPoints -= Damage;
}

bool ADefenseTower::CanFire()
{
	return (_ReloadCountingDown <= 0.0f);
}

void ADefenseTower::DestroyProcess()
{
	if (_bDestroyed) return;	// 이미 처리됐으면 무시
	_bDestroyed = true;

	// 자기 자신 비활성화
	SetActorTickEnabled(false);
	SetActorEnableCollision(false);
	if (_MeshComponent) {
		_MeshComponent->SetVisibility(false);
	}

	// GameMode에게 "타워 파괴됨" 통지 → GameMode가 모든 PC에 Victory UI 송출
	if (_PangaeaGameMode) {
		_PangaeaGameMode->NotifyTowerDestroyed();
	}
}

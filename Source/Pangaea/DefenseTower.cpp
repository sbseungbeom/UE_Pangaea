// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerAvatar.h"
#include "Projectile.h"
#include "Pangaea/PangaeaCharacter.h"
#include "Weapons/Weapon.h"
#include "Kismet/KismetMathLibrary.h"
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
}

// Called every frame
void ADefenseTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_Target != nullptr) {
		Fire();
	}

}

void ADefenseTower::Fire() {
	auto fireball = Cast<AProjectile>(GetWorld()->SpawnActor(_FireballClass));
	FVector startLocation = GetActorLocation();
	startLocation.Z += 100.0f;

	FVector targetLocation = _Target->GetActorLocation();
	targetLocation.Z = startLocation.Z;

	FRotator rotation = UKismetMathLibrary::FindLookAtRotation(startLocation, targetLocation);
	fireball->SetActorLocation(startLocation);
	fireball->SetActorRotation(rotation);
}
void ADefenseTower::OnMeshBeginOverlap(AActor* OtherActor) {
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
			UE_LOG(LogTemp, Warning, TEXT("맞았다!!"));
		}

	}
	else UE_LOG(LogTemp, Warning, TEXT("캐스팅 실패!!!!!!!!!!!!!"));
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

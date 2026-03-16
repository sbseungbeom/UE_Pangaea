

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Locomotion,
	Attack,
	Hit,
	Die
};
/**
 *
 */
UCLASS()
class PANGAEA_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEnemyState State;

	UFUNCTION(BlueprintCallable)
	void OnStateAnimationEnds();
};
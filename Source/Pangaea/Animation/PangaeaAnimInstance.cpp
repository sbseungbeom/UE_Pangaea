// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PangaeaAnimInstance.h"

#include "Character/PangaeaCharacterParent.h"

void UPangaeaAnimInstance::OnStateAnimationEnds() {
	if (State == ECharacterState::Attack) {
		State = ECharacterState::Locomotion;
	}
	auto character = Cast<APangaeaCharacterParent>(GetOwningActor());

	if (State == ECharacterState::Hit) {
		if (character->GetHealthPoints() > 0.0f) {
			State = ECharacterState::Locomotion;
		}
		else {
			State = ECharacterState::Die;
		}
	}

	else if (State == ECharacterState::Die) {
		character->DieProcess();
	}

}
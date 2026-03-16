


#include "EnemyAnimInstance.h"
#include "Enemy.h"

void UEnemyAnimInstance::OnStateAnimationEnds()
{
	if (State == EEnemyState::Attack) {
		State = EEnemyState::Locomotion;
	}
	else
	{
		auto OwnerActor = this->GetOwningActor();
		auto Enemy = Cast<AEnemy>(OwnerActor);

		if (Enemy == nullptr) return;

		if (State == EEnemyState::Hit) {
			if (Enemy->GetHealthPoints() > 0.0f) State = EEnemyState::Locomotion;
			else State = EEnemyState::Die;
		}
		else if (State == EEnemyState::Die) {
			Enemy->DieProcess();
		}
	}
}
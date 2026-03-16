// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Enemy.h"
#include "Enemy/EnemyAIController.h"





void AEnemyAIController::MakeAttackDecision(APawn* TargetPawn) {
	AEnemy* ControlledCharacter = Cast<AEnemy>(GetPawn());
	auto dist = FVector::Dist2D(TargetPawn->GetActorLocation(), GetPawn()->GetTargetLocation());
	
	if (dist <= ControlledCharacter->AttackRange && ControlledCharacter->CanAttack()) {
		ControlledCharacter->Attack();
	}
}
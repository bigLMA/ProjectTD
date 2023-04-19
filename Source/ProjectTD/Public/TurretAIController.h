// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "TurretAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTD_API ATurretAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ATurretAIController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;

	// Sets new target, commands turret to aim and shoot target
	void SetTarget(class AEnemy* NewTarget);

	// Called when setting pawn
	virtual void SetPawn(APawn* InPawn) override;

private:
	// Called when TargetPerceptionUpdated
	UFUNCTION()
	void TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	// Called when enemy left line of sight and controller needs to target new enemy
	UFUNCTION()
	void GetClosestEnemyToTarget(const FVector& Location);

	 AEnemy* Target;
};

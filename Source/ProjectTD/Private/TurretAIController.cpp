// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Enemy.h"
#include "Turret.h"

ATurretAIController::ATurretAIController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer), Target(nullptr)
{
	// Initialize AI perception component
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerception");
	UAISenseConfig_Sight* SightConfig = NewObject<UAISenseConfig_Sight>(this, TEXT("Sight Config"));
	//TSharedPtr<UAISenseConfig_Sight> SightConfig = MakeShared<UAISenseConfig_Sight>();
	SightConfig->SightRadius = 780.f;
	SightConfig->LoseSightRadius = 780.f;
	SightConfig->PeripheralVisionAngleDegrees = 180;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->SetMaxAge(0.001f);
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	AIPerceptionComponent->SetActive(true);
	//delete SightConfig;
}

void ATurretAIController::BeginPlay()
{
	if (AIPerceptionComponent) 
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ATurretAIController::TargetPerceptionUpdated);
	}
}

// Called when setting pawn
void ATurretAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	// Bound looking for enemy when target is lost
	if (auto TurretPawn = Cast<ATurret>(InPawn))
	{
		TurretPawn->OnTargetLost.AddUniqueDynamic(this, &ATurretAIController::GetClosestEnemyToTarget);
		TurretPawn->OnShoot.AddUniqueDynamic(this, &ATurretAIController::ShootAdditionalTargets);
	}
}

// Sets new target, commands turret to aim and shoot target
void ATurretAIController::SetTarget(AEnemy* NewTarget)
{
	Target = NewTarget;

	AActor* TargetActor = Cast<AActor>(NewTarget);
	
	if (auto ControlledTurret = Cast<ATurret>(GetPawn()))
	{
		//if (!IsValid(Target))
		//{
		//	return;
		//}

		//UE_LOG(LogTemp, Warning, TEXT("%s"), *NewTarget->GetName())
		ControlledTurret->AimAt(TargetActor);
	}
}

// Called when TargetPerceptionUpdated
void ATurretAIController::TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	auto Enemy = Cast<AEnemy>(Actor);

	// Check if sense if succesful and target is not set
	if (Stimulus.WasSuccessfullySensed() && Enemy && !Target)
	{
		// Set target
		SetTarget(Enemy);
		return;
	}
	// Check if sense if not succesful and target is pointing at left enemy
	else if (!Stimulus.WasSuccessfullySensed()&&Target == Enemy&& Enemy)	
	{
		// Unset target
		SetTarget(nullptr);

		//Check for other targets
		GetClosestEnemyToTarget(Stimulus.StimulusLocation);
		return;
	}
}

// Called when enemy left line of sight and controller needs to target new enemy
void ATurretAIController::GetClosestEnemyToTarget(const FVector& Location)
{
	TArray<AActor*> DetectedActors;
	AIPerceptionComponent->GetCurrentlyPerceivedActors(UAISenseConfig_Sight::StaticClass(), DetectedActors);

	float MinDistance = 9999999;
	AEnemy* ClosestEnemy = nullptr;

	for (auto DetectedActor : DetectedActors)
	{
		if(auto Enemy = Cast<AEnemy>(DetectedActor))
		{
			float Distance = FVector::Distance(Enemy->GetActorLocation(), Location);

			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				ClosestEnemy = Enemy;
			}
		}
	}

	if (IsValid(ClosestEnemy))
	{
		SetTarget(ClosestEnemy);
	}
	else
	{
		SetTarget(nullptr);
	}
}

// Called when turret is multitarget and shooting
void ATurretAIController::ShootAdditionalTargets(float Radius)
{
	auto ControlledTurret = Cast<ATurret>(GetPawn());

	if (!IsValid(ControlledTurret)) { return; }

	TArray<AActor*> DetectedActors;
	AIPerceptionComponent->GetCurrentlyPerceivedActors(UAISenseConfig_Sight::StaticClass(), DetectedActors);

	int32 Index = 0;

	for (auto DetectedActor : DetectedActors)
	{
		if (auto Enemy = Cast<AEnemy>(DetectedActor))
		{
			// Check if enemy is close enough to target
			if (Enemy->GetDistanceTo(Target) <= Radius&& Enemy!=Target)
			{
				// Fire on enemy
				ControlledTurret->Fire(Enemy);

				// Only two enemies as multitarget targets
				++Index;
				if (Index >= 2) { return; }
			}
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Enemy.generated.h"

UENUM()
enum class EEffects :uint8
{
	Slow, Burning
};

UCLASS()
class PROJECTTD_API AEnemy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemy();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called when enemy takes damage
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void RecieveDamage(int32 Damage);

	// Called when enemy is set on fire
	UFUNCTION(BlueprintCallable, Category = "Burning")
	void ApplyBurning(int32 BurningDamage, float BurningDuration, float BurningInterval);

	// Called when burning damage timer has finished
	UFUNCTION(BlueprintCallable, Category = "Burning")
	void OnBurningDamageTimerFinished();

	// Apply slow on Enemy
	UFUNCTION(BlueprintCallable, Category = "Slow")
	void ApplySlow(float SlowStrength, float SlowDuration);

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Maximum health of enemy
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health", meta = (ClampMin = 1, UIMin = 1))
	int32 MaxHealth;

	// Current health of enemy
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	int32 Health;

	// Bounty value for killing enemy
	UPROPERTY(BlueprintReadOnly, Category = "Rewards", meta = (ClampMin = 1, UIMin = 1))
	int32 Bounty;

	// Base value for calculating random bounty
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rewards", meta = (ClampMin = 1, UIMin = 1))
	int32 BountyBase;

	// Dispersion value for calculating random bounty
	// Use 0 to fix bounty value
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rewards", meta = (ClampMin = 0, UIMin = 0))
	int32 BountyDispersion;

	// Initial speed of enemy
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float InitialSpeed;

	// Enemy copmonents
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Root")
	USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	class USphereComponent* Collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	class UFloatingPawnMovement* FloatingPawnComponent;

private:
	// Called when enemy takes damage over time from burning
	void TakeDamageFromBurning(int32 BurningDamage);

	// Called on BeginPlay to change bounty 
	void RandomizeEnemyBounty();

	// Called on BeginPlay, and when enemy is closed to target
	void WalkToNextTarget();

	// Checks if enemy is closed to target
	void CheckTargetProximity();

	// Used to get location from path
	int TargetIndex;

	// Used to check enemy proximity to target
	float DistanceThreshold;

	// Removes slow from enemy
	void RemoveSlow();

	// Plays when enemy die
	void Die();

	// Reward player with a bounty
	void RewardPlayer();

	// Checks if enemy has that effect
	// Return -1 in index if false
	bool CheckForEffect(EEffects Effect, int32& Index);

	// Sets all enemy variables, that cannot be set on construction
	void InitializeEnemy();

	// Timer handle for checking proximity to target
	FTimerHandle TimerProximityTimerHandle;

	// Timer handle for burning damage
	FTimerHandle BurningDamageTimerHandle;

	// Timer handle for burning effect
	FTimerHandle BurningTimerHandle;

	// Timer handle for slow effect
	FTimerHandle SlowTimerHandle;

	// Enemy effects
	TArray<EEffects>Effects;
};

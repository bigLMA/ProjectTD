// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Enemy.generated.h"

UENUM()
enum class EEffects :uint8
{
	Slow, Burning, Armour_Break, Increased_Bounty
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
	virtual void ReceiveDamage(int32 Damage);

	// Called when enemy is set on fire
	UFUNCTION(BlueprintCallable, Category = "Burning")
	void ApplyBurning(int32 BurningDamage, float BurningDuration, float BurningInterval);

	// Called when burning damage timer has finished
	UFUNCTION(BlueprintCallable, Category = "Burning")
	void OnBurningDamageTimerFinished();

	// Called when enemy armour is broken
	UFUNCTION(BlueprintCallable, Category = "Armour Break")
	void ApplyArmourBreak(int32 ArmourDamage, float BreakDuration);

	// Apply slow on Enemy
	UFUNCTION(BlueprintCallable, Category = "Slow")
	void ApplySlow(float SlowStrength, float SlowDuration);

	// Apply increased bounty on Enemy
	UFUNCTION(BlueprintCallable, Category = "Increased bounty")
	void ApplyIncreasedMoney(int32 NewBounty, float Duration);

	// Regenerates health
	void Regenerate(int32 HealthToAdd);

	float GetHealthPercentage() const;

	FVector GetMeshLocation() const;

	int32 GetDamage()const;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Used to check enemy proximity to target
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Distance Threshold", meta = (ClampMin = 1, UIMin = 1))
	float DistanceThreshold;

	// Maximum health of enemy
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health", meta = (ClampMin = 1, UIMin = 1))
	int32 MaxHealth;

	// Current health of enemy
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	int32 Health;

	// Cuts all damage by flat amount
	UPROPERTY(BlueprintReadOnly, Category = "Armour")
	int32 Armour;

	// Initial enemy armour
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Armour", meta = (ClampMin = -100, UIMin = -100, ClampMan = 100, UIMax = 100))
	int32 InitialArmour;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	int32 DamageToBase;

	// Immunity to negative effects
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Immunities")
	TArray<EEffects> Immunities;

	// Enemy copmonents
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Root")
	USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	class USphereComponent* Collision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	class UFloatingPawnMovement* FloatingPawnComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	class UWidgetComponent* HealthComponent;

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

	// Removes slow from enemy
	void RemoveSlow();

	// Removes armour break
	void RemoveArmourBreak();

	// Removes increased bounty
	void RemoveIncreasedBounty();

	// Plays when enemy die
	void Die();

	// Reward player with a bounty
	void RewardPlayer();

	// Checks if enemy has that effect
	// Return -1 in index if false
	bool CheckForEffect(EEffects Effect, int32& Index);

	// Sets all enemy variables, that cannot be set on construction
	void InitializeEnemy();

	// Checks for immunity
	bool CheckForImmunity(EEffects Effect, int32& Index);

	// Increased bounty
	int32 IncreasedBounty;

	// Timer handle for checking proximity to target
	FTimerHandle TimerProximityTimerHandle;

	// Timer handle for burning damage
	FTimerHandle BurningDamageTimerHandle;

	// Timer handle for burning effect
	FTimerHandle BurningTimerHandle;

	// Timer handle for slow effect
	FTimerHandle SlowTimerHandle;

	// Timer handle for armour break effect
	FTimerHandle ArmourBreakTimerHandle;

	// Timer handle for increased bounty
	FTimerHandle IncreasedBountyTimerHandle;

	// Enemy effects
	TArray<EEffects>Effects;
};

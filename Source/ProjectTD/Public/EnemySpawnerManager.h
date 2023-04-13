// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnerManager.generated.h"

USTRUCT(BlueprintType)
struct FSpawnWave
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<class AEnemy>> EnemyClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 30, UIMin = 30, ClampMax = 600, UIMax = 600))
	int32 SecondsUntilNextWave;
};

UCLASS()
class PROJECTTD_API AEnemySpawnerManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawnerManager();

	// Begin spawn next wave
	void BeginSpawnWave();

	// Set timer to countdown to spawn next wave
	UFUNCTION(BlueprintCallable, Category = "Wave spawning")
	void SetTimerToSpawnNextWave(int32 DelayTime);

protected:
	// Spawning properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TArray<FSpawnWave> SpawnWaves;

	// Interwal to wait between spawn next enemy
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (ClampMin = 0.4, UIMin = 0.4, ClampMax = 2.5, UIMax = 2.5))
	float SpawnInterval;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Spawning info internal
	int32 CurrentWave;
	int32 CurrentEnemy;
	int32 CurrentSecondsToNextWave;

	// Timers
	FTimerHandle InWaveTimerHandle;
	FTimerHandle BetweenWaveTimerHandle;

	// Spawns next enemy in queue
	void SpawnEnemy();

	// Counts down to spawn next wave
	void SetCountdownToSpanwWave();
};

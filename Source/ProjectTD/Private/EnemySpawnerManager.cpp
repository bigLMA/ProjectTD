// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawnerManager.h"
#include "Enemy.h"

// Sets default values
AEnemySpawnerManager::AEnemySpawnerManager()
	:InitialDelay(8), CurrentWave(-1), CurrentEnemy(0), CurrentSecondsToNextWave(0), SpawnInterval(0.65)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEnemySpawnerManager::BeginPlay()
{
	Super::BeginPlay();
	
	SetTimerToSpawnNextWave(InitialDelay);
	//BeginSpawnWave();
}

// Begin spawn next wave
void AEnemySpawnerManager::BeginSpawnWave()
{
	if (CurrentWave+1 >= SpawnWaves.Num())
	{
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
		return;
	}

	CurrentEnemy = 0;
	++CurrentWave;

	// Release old timers...
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	// Check if all waves are spawned
	if (!SpawnWaves.IsValidIndex(CurrentWave))
	{
		return;
	}

	//... and start new one
	GetWorld()->GetTimerManager().SetTimer(InWaveTimerHandle, this,
		&AEnemySpawnerManager::SpawnEnemy, SpawnInterval, true);
	SetTimerToSpawnNextWave(SpawnWaves[CurrentWave].SecondsUntilNextWave);
}

// Set timer to countdown to spawn next wave
void AEnemySpawnerManager::SetTimerToSpawnNextWave(int32 DelayTime)
{
	CurrentSecondsToNextWave = DelayTime;
	GetWorld()->GetTimerManager().ClearTimer(BetweenWaveTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(BetweenWaveTimerHandle, this,
		&AEnemySpawnerManager::SetCountdownToSpanwWave,	1, true);
}

int32 AEnemySpawnerManager::GetCurrentWave() const
{
	return CurrentWave;
}

int32 AEnemySpawnerManager::GetCurrentEnemy() const
{
	return CurrentEnemy;
}

int32 AEnemySpawnerManager::GetSecondsRemaining() const
{
	return CurrentSecondsToNextWave;
}

// Spawns next enemy in queue
void AEnemySpawnerManager::SpawnEnemy()
{
	// Check if spawned all enemies in wave
	if (!SpawnWaves.IsValidIndex(CurrentWave)||
		CurrentEnemy >= SpawnWaves[CurrentWave].EnemyClasses.Num() ||
		!SpawnWaves[CurrentWave].EnemyClasses.IsValidIndex(CurrentEnemy))
	{
		GetWorld()->GetTimerManager().ClearTimer(InWaveTimerHandle);
		return;
	}

	// Spawn enemy
	FActorSpawnParameters Params;
	auto Location = GetActorLocation();
	GetWorld()->SpawnActor<AEnemy>(SpawnWaves[CurrentWave].EnemyClasses[CurrentEnemy++],
		Location, FRotator(), Params);
}

// Counts down to spawn next wave
void AEnemySpawnerManager::SetCountdownToSpanwWave()
{
	--CurrentSecondsToNextWave;

	if (CurrentSecondsToNextWave <= 0)
	{
		BeginSpawnWave();
	}
}

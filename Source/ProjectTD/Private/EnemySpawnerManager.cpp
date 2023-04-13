// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawnerManager.h"
#include "Enemy.h"

// Sets default values
AEnemySpawnerManager::AEnemySpawnerManager()
	: CurrentWave(-1), CurrentEnemy(0), CurrentSecondsToNextWave(0), SpawnInterval(0.65)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEnemySpawnerManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Begin spawn next wave
void AEnemySpawnerManager::BeginSpawnWave()
{
	++CurrentWave;
	CurrentEnemy = 0;

	// Release old timers...
	GetWorld()->GetTimerManager().ClearTimer(InWaveTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(BetweenWaveTimerHandle);

	//... and start new one
	GetWorld()->GetTimerManager().SetTimer(InWaveTimerHandle, this,
		&AEnemySpawnerManager::SpawnEnemy, SpawnInterval, true, SpawnInterval);
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

// Spawns next enemy in queue
void AEnemySpawnerManager::SpawnEnemy()
{
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

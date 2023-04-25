// Fill out your copyright notice in the Description page of Project Settings.


#include "Base.h"
#include "Components/BoxComponent.h"
#include "Enemy.h"
#include "CameraPlayer.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABase::ABase()
	:MaxHealth(100)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Initialize components
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	Base = CreateDefaultSubobject<UStaticMeshComponent>("Base");
	Base->SetupAttachment(Root);

	DamageCollision = CreateDefaultSubobject<UBoxComponent>("Shoot collision");
	DamageCollision->SetupAttachment(Root);
}

float ABase::GetHealthPercentage() const
{
	return static_cast<float>(Health)/ static_cast<float>(MaxHealth);
}

// Called when the game starts or when spawned
void ABase::BeginPlay()
{
	Super::BeginPlay();

	// Bind event with component overlapped
	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &ABase::OnComponentOverlap);

	// Set Health value from Max Health
	Health = MaxHealth;

	// Set player pointer
	PlayerPawn = Cast<ACameraPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

// Called when DamageCollision is overlapped
void ABase::OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if enemy is overlapping collision
	if (auto Enemy = Cast<AEnemy>(OtherActor))
	{
		RecieveDamage(Enemy->GetDamage());
		OtherActor->Destroy();
	}
}

// Called when base is damaged
void ABase::RecieveDamage(int32 Damage)
{
	Health -= Damage;
	UE_LOG(LogTemp, Warning, TEXT("Health %d"), Health);

	if (PlayerPawn)
	{
		PlayerPawn->UpdateBaseHealth(GetHealthPercentage());
	}

	if (Health <= 0)
	{
		BaseDestroyed();
	}
}

// Called when base is destroyed
void ABase::BaseDestroyed()
{
	//TODO when base is destroyed
	Destroy();
}


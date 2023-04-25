// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Base.generated.h"

UCLASS()
class PROJECTTD_API ABase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABase();

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthPercentage() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Health of the base
	UPROPERTY(BlueprintReadOnly, Category = "Health", meta = (ClampMin=1, UIMin=1))
	int32 Health;

	// MaxHealth of the base
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health", meta = (ClampMin = 1, UIMin = 1))
	int32 MaxHealth;

	// Player
	UPROPERTY(BlueprintReadOnly, Category = "Player")
	class ACameraPlayer* PlayerPawn;

	// Components
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Root")
	USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* Base;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage Collision")
	class UBoxComponent* DamageCollision;

private:
	// Called when DamageCollision is overlapped
	UFUNCTION()
	void OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, 
		const FHitResult& SweepResult);

	// Called when base is damaged
	void RecieveDamage(int32 Damage);

	// Called when base is destroyed
	void BaseDestroyed();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "ShieldComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTD_API UShieldComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	UShieldComponent();

	// When shield takes damage
	void RecieveDamage(int32 Damage);

protected:

	// Shield max health
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health", meta = (ClampMin = 1, UIMin = 1))
	float MaxHealth;

	// Shield health
	UPROPERTY(BlueprintReadOnly, Category = "Health", meta = (ClampMin=1, UIMin= 1))
	float Health;

	// How much shield regenerates per second
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recharge", meta = (ClampMin = 0.1, UIMin = 0.1))
	float ShieldRegenerationRate;

	UFUNCTION(BlueprintCallable, Category = "Regeneration")
	void SetShieldRegeneration();

private:
	UFUNCTION()
	void RegenerateShield(float DeltaTime);
};

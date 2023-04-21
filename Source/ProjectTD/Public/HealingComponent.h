// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "HealingComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTD_API UHealingComponent : public USphereComponent
{
	GENERATED_BODY()
	
public:
	UHealingComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Amount of health regenerated per interval
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Healing", meta = (ClampMin = 1, UIMin = 1))
	int32 Regeneration;

	// Healing interval
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Healing", meta = (ClampMin = 0.05f, ClampMin = 0.05f))
	float RegenInterval;

private:
	void Regenerate();
};

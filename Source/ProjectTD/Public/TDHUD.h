// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TDHUD.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class PROJECTTD_API UTDHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Called when base is attacked
	void DisplayBaseHealth(float Percentage);

	// Called when player earns/spends money
	void DisplayMoney(int32 Money);

protected:
	// Progress bar to display health
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MoneyBlock;
};

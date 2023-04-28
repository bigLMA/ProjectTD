// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TDHUD.generated.h"

class ATurret;

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

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Building menu")
	void BuildingMenuPull(bool bPullStatus);

	void ToggleTurretInfo(bool bVisible, ATurret* InTurret);

protected:
	// Progress bar to display health
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MoneyBlock;

	UPROPERTY(meta = (BindWidget))
	class UBuildingMenu* BuildingMenu;

	UPROPERTY(meta = (BindWidget))
	class UTurretPanel* TurretPanel;

	virtual void NativePreConstruct() override;
};

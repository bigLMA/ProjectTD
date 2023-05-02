// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TurretUpgradeInfo.generated.h"

class ATurret;

/**
 * 
 */
UCLASS()
class PROJECTTD_API UTurretUpgradeInfo : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void DisplayWidget(ATurret* InTurret, int32 InIndex);

protected:
	virtual void NativePreConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* UpgradeBar;

	UPROPERTY(meta = (BindWidget))
	class UButton* UpgradeButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CostLabel;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DescriptionLabel;

	UPROPERTY(meta = (BindWidget))
	class UImage* UpgradeImage;

private:
	// Index of widget to navigate
	// through Turret Upgrades
	int32 Index;

	ATurret* Turret;

	UFUNCTION()
	void OnButtonPushed();

	//TODO info widget
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TurretPanel.generated.h"

class ATurret;

/**
 * 
 */
UCLASS()
class PROJECTTD_API UTurretPanel : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void DispalyPanel(ATurret* InTurret);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	class UImage* TurretImage;

	// Panel with all turret upgrades
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* UpgradeBox;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TurretName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TurretDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TurretUpgradeClass")
	TSubclassOf<class UTurretUpgradeInfo> TurretUpgradeClass;

private:
	ATurret* Turret;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildingSlot.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTD_API UBuildingSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;

	// Called when mouse is down on widget
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret class")
	TSubclassOf<class ATurret> TurretClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Preview actor")
	TSubclassOf <class ATurretPrevievActor> PreviewClass;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CostLabel;

private:
	int32 Cost;

	// TODO INFO WIDGET
};

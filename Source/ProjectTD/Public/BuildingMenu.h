// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildingMenu.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPullButtonPressed, bool, bPullStatus)

/**
 * 
 */
UCLASS()
class PROJECTTD_API UBuildingMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FOnPullButtonPressed OnPullButtonPressed;

protected:
	virtual void NativePreConstruct() override;

	// Status of menu being pulled out/in
	// False means menu is "hidden"
	UPROPERTY(BlueprintReadWrite)
	bool bMenuPullStatus;

	// Grid where all slots will be displayed
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* BuildingGrid;

	// Button to trigger widget animation
	UPROPERTY(meta = (BindWidget))
	class UButton* PullButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PullButtonText;

private:
	// Called when pull button is pressed
	// to begin animation in HUD
	UFUNCTION()
	void PullButtonPressed();

	void SetPullButtonText();
};

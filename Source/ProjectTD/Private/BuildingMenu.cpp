// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingMenu.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UBuildingMenu::NativePreConstruct()
{
	Super::NativePreConstruct();

	bMenuPullStatus = false;

	if (PullButton)
	{
		PullButton->OnClicked.AddUniqueDynamic(this, &UBuildingMenu::PullButtonPressed);
		SetPullButtonText();
	}
}
	
// Called when pull button is pressed
// to begin animation in HUD
void UBuildingMenu::PullButtonPressed()
{
	bMenuPullStatus = !bMenuPullStatus;

	if (PullButtonText)
	{
		SetPullButtonText();
	}

	OnPullButtonPressed.Broadcast(bMenuPullStatus);
}

void UBuildingMenu::SetPullButtonText()
{
	FText Arrow = (bMenuPullStatus) ? FText::FromString("<-") : FText::FromString("->");
	PullButtonText->SetText(Arrow);
}

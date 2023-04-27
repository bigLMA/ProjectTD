// Fill out your copyright notice in the Description page of Project Settings.


#include "TDHUD.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "BuildingMenu.h"

// Called when base is attacked
void UTDHUD::DisplayBaseHealth(float Percentage)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(Percentage);
	}
}

// Called when player earns/spends money
void UTDHUD::DisplayMoney(int32 Money)
{
	if (MoneyBlock)
	{
		MoneyBlock->SetText(FText::AsNumber(Money));
	}
}

void UTDHUD::NativePreConstruct()
{
	if (BuildingMenu)
	{
		BuildingMenu->OnPullButtonPressed.AddUniqueDynamic(this, &UTDHUD::BuildingMenuPull);
	}
}

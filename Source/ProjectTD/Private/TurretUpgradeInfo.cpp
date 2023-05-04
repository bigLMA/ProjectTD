// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretUpgradeInfo.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Turret.h"
#include "Kismet/GameplayStatics.h"
#include "CameraPlayer.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"

void UTurretUpgradeInfo::DisplayWidget(ATurret* InTurret, int32 InIndex)
{
	if (!InTurret) { return; }

	Index = InIndex;
	Turret = InTurret;

	auto Upgrade = Turret->GetUpgradeAtIndex(Index);

	CostLabel->SetText(FText::FromString(FString::FromInt(Upgrade.UpgradeCost->GetFloatValue(Upgrade.CurrentLevel + 1))));
	DescriptionLabel->SetText(Upgrade.Description);
	if (Upgrade.Icon)
	{
		UpgradeImage->SetBrushFromTexture(Upgrade.Icon);
	}
	UpgradeBar->SetPercent(static_cast<float>(Upgrade.CurrentLevel) / static_cast<float>(Upgrade.MaxLevel));
}

void UTurretUpgradeInfo::NativePreConstruct()
{
	if (UpgradeButton)
	{
		UpgradeButton->OnClicked.AddUniqueDynamic(this, &UTurretUpgradeInfo::OnButtonPushed);
	}
}

void UTurretUpgradeInfo::OnButtonPushed()
{
	if (!Turret) { return; }

	auto Upgrade = Turret->GetUpgradeAtIndex(Index);

	// Check if is fully Upgraded
	if (Upgrade.CurrentLevel >= Upgrade.MaxLevel)
	{
		CostLabel->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	//Check if player has money to Upgrade
	if (auto Player = Cast<ACameraPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		if (Player->GetMoney() < Upgrade.UpgradeCost->GetFloatValue(Upgrade.CurrentLevel + 1))
		{
			return;
		}
	}

	// Otherwise upgrade turret
	Turret->Upgrade(Upgrade.UpgradeType);
	DisplayWidget(Turret, Index);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHealthWidget.h"
#include "Components/ProgressBar.h"
#include "Enemy.h"

void UEnemyHealthWidget::SetOwner(AEnemy* InEnemy)
{
	Owner = InEnemy;
}

void UEnemyHealthWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (Owner)
	{
		HealthBar->SetPercent(Owner->GetHealthPercentage());
	}
}

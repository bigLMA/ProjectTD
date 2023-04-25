// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTD_API UEnemyHealthWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetOwner(class AEnemy* InEnemy);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

private:
	AEnemy* Owner;
};

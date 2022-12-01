// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PostApocaPlayerController.generated.h"

class UUserWidget;

UCLASS()
class THEDEADLYFOREST_API APostApocaPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

public:
	virtual void GameHasEnded(AActor* EndGameFocus, bool bIsWinner) override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> MainGameWidget;

	UPROPERTY()
	UUserWidget* HUD;

	
};

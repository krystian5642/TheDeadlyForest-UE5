// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheDeadlyForestGameModeBase.h"
#include "KillAndSurviveGameMode.generated.h"

/**
 * 
 */
UCLASS()
class THEDEADLYFOREST_API AKillAndSurviveGameMode : public ATheDeadlyForestGameModeBase
{
	GENERATED_BODY()

public:
	virtual void PawnKilled(APawn* DeadPawn);

private:
	void EndGame(bool bIsPlayerWinner);
	
};

// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TheDeadlyForestGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class THEDEADLYFOREST_API ATheDeadlyForestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void PawnKilled(APawn* DeadPawn);
	
};

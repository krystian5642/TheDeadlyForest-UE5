// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_UpdateRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class THEDEADLYFOREST_API UBTService_UpdateRandomLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_UpdateRandomLocation();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	float SearchAreaRadius = 20000.f;

};

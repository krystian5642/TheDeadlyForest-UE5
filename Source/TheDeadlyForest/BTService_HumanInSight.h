// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_HumanInSight.generated.h"

/**
 * 
 */
UCLASS()
class THEDEADLYFOREST_API UBTService_HumanInSight : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_HumanInSight();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	

};

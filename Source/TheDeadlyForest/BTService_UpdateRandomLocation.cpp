// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdateRandomLocation.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"

UBTService_UpdateRandomLocation::UBTService_UpdateRandomLocation()
{
    NodeName=TEXT("UpdateRandomLocation");
}

void UBTService_UpdateRandomLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp,NodeMemory,DeltaSeconds);
    APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if(ControlledPawn && BlackboardComp && NavSys)
    {
        FNavLocation RandomNavLocation;
        FVector PawnLocation = ControlledPawn->GetActorLocation();
        NavSys->GetRandomReachablePointInRadius
        (
            PawnLocation,
            SearchAreaRadius,
            RandomNavLocation
        );
        BlackboardComp->SetValueAsVector(GetSelectedBlackboardKey(),RandomNavLocation.Location);
    }
}

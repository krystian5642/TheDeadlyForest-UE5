// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_HumanInSight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"

UBTService_HumanInSight::UBTService_HumanInSight()
{
    NodeName = TEXT("HumanInSight");
}

void UBTService_HumanInSight::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp,NodeMemory,DeltaSeconds);
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this,0);
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    if(PlayerPawn && BlackboardComp)
    {
        BlackboardComp->SetValueAsVector(GetSelectedBlackboardKey(),PlayerPawn->GetActorLocation());
    }
}
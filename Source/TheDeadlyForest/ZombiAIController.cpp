// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombiAIController.h"


void AZombiAIController::BeginPlay()
{
    Super::BeginPlay();
    if(AIBehaviorTree)
    {
        RunBehaviorTree(AIBehaviorTree);
    }
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "KillAndSurviveGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"

void AKillAndSurviveGameMode::PawnKilled(APawn* DeadPawn)
{
    Super::PawnKilled(DeadPawn);

    AController* PawnController = DeadPawn->GetController();
    if(PawnController)
    {
        EndGame(true);
    }
}

void AKillAndSurviveGameMode::EndGame(bool bIsPlayerWinner)
{
    for(AController* Controller : TActorRange<AController>(GetWorld()))
    {
        bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
        Controller->GameHasEnded(Controller->GetPawn(),bIsWinner);
    }
}



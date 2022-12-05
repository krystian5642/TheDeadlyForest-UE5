// Fill out your copyright notice in the Description page of Project Settings.


#include "KillAndSurviveGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"

void AKillAndSurviveGameMode::PawnKilled(APawn* DeadPawn)
{
    Super::PawnKilled(DeadPawn);

    APlayerController* PlayerController = DeadPawn->GetController<APlayerController>();
    if(PlayerController)
    {
        EndGame(false);
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



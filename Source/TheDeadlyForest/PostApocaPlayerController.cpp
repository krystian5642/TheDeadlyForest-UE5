// Fill out your copyright notice in the Description page of Project Settings.


#include "PostApocaPlayerController.h"
#include "Blueprint/UserWidget.h"

void APostApocaPlayerController::BeginPlay()
{
    Super::BeginPlay();
    HUD = CreateWidget(this,MainGameWidget);
    if(HUD)
    {
        HUD->AddToViewport();
    }
}

void APostApocaPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus,bIsWinner);

}



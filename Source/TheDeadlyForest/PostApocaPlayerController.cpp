// Fill out your copyright notice in the Description page of Project Settings.


#include "PostApocaPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"

void APostApocaPlayerController::BeginPlay()
{
    Super::BeginPlay();
    HUD = CreateWidget(this,HUDWidgetClass);
    if(HUD)
    {
        HUD->AddToViewport();
    }
}

void APostApocaPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus,bIsWinner);
    if(HUD)
    {
        HUD->RemoveFromParent();
    }
    FTimerHandle GameOverWidgetDelayTimer;
    GetWorld()->GetTimerManager().SetTimer
    (
        GameOverWidgetDelayTimer,
        this,
        &APostApocaPlayerController::DisplayGameOverWidget,
        1.5,
        false
    );
}

void APostApocaPlayerController::DisplayGameOverWidget()
{
    GameOver = CreateWidget(this,GameOverWidget);
    if(GameOver)
    {
        GameOver->AddToViewport();
    }
}



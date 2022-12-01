// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicZombie.h"
#include "Components/CapsuleComponent.h"
#include "KillAndSurviveGameMode.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABasicZombie::ABasicZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HeadHitCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HeadHitCapsule"));
	USkeletalMeshComponent* CharacterMesh = GetMesh();
	if(CharacterMesh)
	{
		HeadHitCapsule->AttachToComponent
		(
			CharacterMesh,
			FAttachmentTransformRules::KeepRelativeTransform,
			TEXT("Head")
		);
	}
}

// Called when the game starts or when spawned
void ABasicZombie::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth=MaxHealth;
}

// Called every frame
void ABasicZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ABasicZombie::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount,DamageEvent,EventInstigator,DamageCauser);
	if(DamageAmount<=0)
	{
		return 0;
	}
	float DamageApplied = FMath::Min(CurrentHealth,DamageAmount);
	CurrentHealth-=DamageApplied;
	const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
	LastShotDirection=PointDamageEvent->ShotDirection;
	if(!IsAlive())
	{
		AKillAndSurviveGameMode* GameMode = GetWorld()->GetAuthGameMode<AKillAndSurviveGameMode>();
		if(GameMode)
		{
			SetAnimationZombieDeath();
			GameMode->PawnKilled(this);
		}
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	return DamageApplied;
}


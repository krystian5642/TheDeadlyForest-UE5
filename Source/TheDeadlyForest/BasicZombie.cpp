// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicZombie.h"

// Sets default values
ABasicZombie::ABasicZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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
	if(DamageAmount<0)
	{
		return 0;
	}
	float DamageApplied = FMath::Min(CurrentHealth,DamageAmount);
	CurrentHealth-=DamageApplied;
	return DamageApplied;
}


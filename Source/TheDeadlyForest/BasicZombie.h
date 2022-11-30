// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasicZombie.generated.h"

UCLASS()
class THEDEADLYFOREST_API ABasicZombie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicZombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	inline bool IsAlive() const{return CurrentHealth<=0;}

private:
	UPROPERTY(EditDefaultsOnly,Category="Health")
	float MaxHealth =100.f;

	UPROPERTY(VisibleAnywhere,Category="Health")
	float CurrentHealth = 0.f;

};

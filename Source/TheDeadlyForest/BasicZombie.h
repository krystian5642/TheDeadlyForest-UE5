// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasicZombie.generated.h"

class UBoxComponent;

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

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	FVector LastShotDirection = FVector(0,0,0);

	//It sets if a zombie has been killed by either front or back shot
	//Since we have 2 death animations 
	UFUNCTION(BlueprintImplementableEvent)
	void SetAnimationZombieDeath();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	inline bool IsAlive() const{return CurrentHealth>0;}

	inline float GetCurrentHealth() const {return CurrentHealth;} 

private:
	UPROPERTY(EditDefaultsOnly,Category="Health")
	float MaxHealth =100.f;

	UPROPERTY(VisibleAnywhere,Category="Health")
	float CurrentHealth = 0.f;

	//When head is hit by bullet, zombie dies
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* HeadHitBox;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PostApocaCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AGun;

UENUM(BlueprintType)
enum class EBasicMovementMode : uint8
{
	Standing,
	Walking,
	Running
};

UCLASS()
class THEDEADLYFOREST_API APostApocaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APostApocaCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Movement")
	EBasicMovementMode BasicMovementMode = EBasicMovementMode::Standing;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Movement")
	float TargetSpeed=0;

	UFUNCTION(BlueprintImplementableEvent)
	void AdjustMaxWalkSpeed();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	inline bool HasAGun() const {return Gun!=nullptr;}

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);
	void ChangeBasicMovementMode(float AxisValue);

	UPROPERTY(EditAnywhere,Category="Gamepad")
	float ControllerRotationRate = 20.f;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere,Category="Weapon")
	TSubclassOf<AGun> GunClass;

	UPROPERTY()
	AGun* Gun=nullptr;

};

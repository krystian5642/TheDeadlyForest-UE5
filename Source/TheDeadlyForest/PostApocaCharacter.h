// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Containers/ContainerAllocationPolicies.h"
#include "PostApocaCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AGun;

UENUM(BlueprintType)
enum class EBasicMovementMode : uint8
{
	Standing,
	Walking,
	Running,
	Aiming
};

UENUM(BlueprintType)
enum class ECameraMode : uint8
{
	ThirdPersonClose,
	ThirdPersonFar,
	FirstPerson
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

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	FTransform LeftHandTransform;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Combat")
	bool bIsPlayerAiming = false;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Camera")
	ECameraMode CameraMode = ECameraMode::ThirdPersonClose;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	inline bool HasAGun() const {return CurrentWeapon!=nullptr;}

private:
	//Basic movement and camera rotation
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);
	void ChangeBasicMovementMode(float AxisValue);
	void Jump();

	//Other functions
	void UpdateLeftHandTransform();
	void StartAiming();
	void EndAiming();
	void ChangeCameraMode();
	void PullTrigger();
	void ToFirstPersonCamera(bool bFirstPerson);
	void SetAiming(bool bIsAiming);
	void SwitchWeaponUp();
	void SwitchWeaponDown();
	void EnableNewWeapon();

	//Because bUseControllerRotationYaw is set to false when we finish aiming
	void ControlCameraMode();

	UPROPERTY(EditAnywhere,Category="Gamepad")
	float ControllerRotationRate = 20.f;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ThirdPersonCamera;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FirstPersonCamera;

	UPROPERTY(EditAnywhere,Category="Weapon")
	TArray<TSubclassOf<AGun>> WeaponClasses;

	UPROPERTY(VisibleAnywhere,Category="Weapon")
	int32 WeaponsNumber=0;

	UPROPERTY(VisibleAnywhere,Category="Weapon")
	int32 CurrentWeaponIndex=0;

	UPROPERTY(BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	AGun* CurrentWeapon = nullptr;

	UPROPERTY()
	TArray<AGun*> AllAvailableWeapons;

	UPROPERTY(EditDefaultsOnly,Category="Health")
	float MaxHealth =100.f;

	UPROPERTY(VisibleAnywhere,Category="Health")
	float CurrentHealth = 0.f;

};

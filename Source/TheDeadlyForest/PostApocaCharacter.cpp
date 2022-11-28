// Fill out your copyright notice in the Description page of Project Settings.


#include "PostApocaCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APostApocaCharacter::APostApocaCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    Camera  = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	USkeletalMeshComponent* CharacterMesh = GetMesh();
	if(CharacterMesh)
	{
		SpringArm->SetupAttachment(CharacterMesh);
    	Camera->SetupAttachment(SpringArm);
	}
}

// Called when the game starts or when spawned
void APostApocaCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APostApocaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AdjustMaxWalkSpeed();
}

// Called to bind functionality to input
void APostApocaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Mouse
	PlayerInputComponent->BindAxis(TEXT("LookUp"),this,&APostApocaCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"),this,&APostApocaCharacter::AddControllerYawInput);

	//Gamepad
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"),this,&APostApocaCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"),this,&APostApocaCharacter::LookRightRate);

	//Keyboard
	PlayerInputComponent->BindAxis(TEXT("MoveForward"),this,&APostApocaCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"),this,&APostApocaCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("MovementMode"),this,&APostApocaCharacter::ChangeBasicMovementMode);

	//Spacebar and right trigger on the gamepad
	PlayerInputComponent->BindAction(TEXT("Jump"),EInputEvent::IE_Pressed,this,&APostApocaCharacter::Jump);

}

void APostApocaCharacter::MoveForward(float AxisValue)
{
	if ((Controller != nullptr) && (AxisValue != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = UKismetMathLibrary::GetForwardVector(YawRotation);
		AddMovementInput(Direction, AxisValue);
	}
}

void APostApocaCharacter::MoveRight(float AxisValue)
{
	if ((Controller != nullptr) && (AxisValue != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector
		const FVector Direction = UKismetMathLibrary::GetRightVector(YawRotation);
		AddMovementInput(Direction, AxisValue);
	}
}

void APostApocaCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * ControllerRotationRate * GetWorld()->GetDeltaSeconds());
}

void APostApocaCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * ControllerRotationRate * GetWorld()->GetDeltaSeconds());
}

void APostApocaCharacter::ChangeBasicMovementMode(float AxisValue)
{
	const FVector Velocity = GetVelocity();
	float GroundSpeed = FVector(Velocity.X,Velocity.Y,0).Size();


	if(GroundSpeed == 0 && BasicMovementMode != EBasicMovementMode::Standing)
	{
		BasicMovementMode = EBasicMovementMode::Standing;
	}
	else if(BasicMovementMode == EBasicMovementMode::Standing && GroundSpeed)
	{
		BasicMovementMode = EBasicMovementMode::Walking;
	}
	else if(BasicMovementMode == EBasicMovementMode::Walking && AxisValue)
	{
		BasicMovementMode = EBasicMovementMode::Running;
	}
	else if(BasicMovementMode == EBasicMovementMode::Running && !AxisValue)
	{
		BasicMovementMode = EBasicMovementMode::Walking;
	}
}

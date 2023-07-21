// Fill out your copyright notice in the Description page of Project Settings.


#include "RpgCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h" 
#include "Camera/CameraComponent.h" 
#include "GameFramework/CharacterMovementComponent.h" 

ARpgCharacter::ARpgCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
}

void ARpgCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(CharacterMappingContext, 0);
		}
	}
}

//Functions trigged by input

// Move controller around
void ARpgCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	// Pitch and yaw
	const FRotator Rotation = /*Controller->*/GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	// add movement on Y Axis, normalized vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);
	
	// add movement on X Axis, normalized vector
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

// Look around
void ARpgCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if (GetController()) {

		UE_LOG(LogTemp, Warning, TEXT("Look around log"));

		// rotate camera
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

// Jump Action
void ARpgCharacter::Jump(const FInputActionValue& Value)
{
	bool isKeyPressed = Value.Get<bool>();
	if (isKeyPressed && GetCharacterMovement()->IsMovingOnGround()) {
		ACharacter::Jump();
	}
}

// input section end

void ARpgCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Setup input and bind actions
void ARpgCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARpgCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARpgCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ARpgCharacter::Jump);
	}
}


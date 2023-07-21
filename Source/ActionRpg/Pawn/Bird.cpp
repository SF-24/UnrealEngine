#pragma once

#include "Bird.h"
#include "Components/SkeletalMeshComponent.h" 
#include "Components/CapsuleComponent.h" 
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h" 
#include "Camera/CameraComponent.h" 

ABird::ABird()
{
	PrimaryActorTick.bCanEverTick = true;

	// Setup capsule component
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetCapsuleHalfHeight(20.0f);
	Capsule->SetCapsuleRadius(15.0f);
	SetRootComponent(Capsule);

	// Setup skeletal mesh
	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMesh"));
	BirdMesh->SetupAttachment(GetRootComponent());

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	//SpringArm->TargetArmLength = 210.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	AutoPossessPlayer = EAutoReceiveInput::Player0;



}

void ABird::BeginPlay()
{
	Super::BeginPlay();

	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(BirdMappingContext, 0);
		}
	}
}

 
// Move around
void ABird::Move(const FInputActionValue& Value)
{
	const float DirectionValue = Value.Get<float>();
	if (DirectionValue) {
		FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, DirectionValue);
	}
}

// Look around
void ABird::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if (GetController()) {

		UE_LOG(LogTemp, Warning, TEXT("Look around log"));

		// rotate camera
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}	

void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABird::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABird::Look);
	}
}


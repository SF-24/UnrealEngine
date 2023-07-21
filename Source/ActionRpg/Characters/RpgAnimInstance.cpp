// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RpgAnimInstance.h"
#include "RpgCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void URpgAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	RpgCharacter = Cast<ARpgCharacter>(TryGetPawnOwner());
	if (RpgCharacter) {
		RpgCharacterMovement = RpgCharacter->GetCharacterMovement();
	}
}

void URpgAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (RpgCharacterMovement) {
		GroundSpeed = UKismetMathLibrary::VSizeXY(RpgCharacterMovement->Velocity);
		IsFalling = RpgCharacterMovement->IsFalling();
	}
}

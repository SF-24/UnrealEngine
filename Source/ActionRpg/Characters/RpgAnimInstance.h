// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RpgAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API URpgAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	class ARpgCharacter* RpgCharacter;

	UPROPERTY(BlueprintReadOnly, Category=Movement)
	class UCharacterMovementComponent* RpgCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category=Movement)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsFalling;

};

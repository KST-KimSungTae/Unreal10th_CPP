// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ActionCharacter.generated.h"


class UInputAction;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class UNREAL10TH_CPP_API AActionCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActionCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	void OnTestAction(const FInputActionValue& Value);

protected:
	void OnMoveAction(const FInputActionValue& Value);

protected:
	void OnBoostAction(const FInputActionValue& Value);

protected:
	void OnBoostEnd(const FInputActionValue& Value);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction>IA_Test;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction>IA_Move;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction>IA_Boost;

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> CameraSpringArmComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> CameraComponent = nullptr;

};

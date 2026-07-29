// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/PickUpBase.h"
#include "PickUpEffect.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL10TH_CPP_API APickUpEffect : public APickUpBase
{
	GENERATED_BODY()
protected:
	virtual void OnPickup(AActor* InTarget) override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Stamina = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = 0.0f;
	
};

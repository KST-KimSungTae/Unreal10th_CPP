// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UStatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREAL10TH_CPP_API IStatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stat")
	UStatComponent* GetStatComponent() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Health")
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Health")
	void Damaged(float InAmount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Health")
	void RecoveryHealth(float InAmount);
};

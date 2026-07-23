// Fill out your copyright notice in the Description page of Project Settings.


#include "StatComponent.h"

// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UStatComponent::InitializeStat(FAutoRecoveryData& InData)
{
	CurrentStamina = MaxStamina;
	CurrentHealth = MaxHealth;
	StaminaRecoveryData = InData;
}

float UStatComponent::GetCurrentStamina_Implementation() const
{
	return CurrentStamina;
}

bool UStatComponent::ConsumeStamina_Implementation(float InAmount)
{
	bool bResult = false;
	if (CurrentStamina >= InAmount)
	{
		CurrentStamina -= InAmount;

		//StaminaAutoRecoverySecond = StaminaRecoveryCoolTime;

		FTimerManager& TimerManager = GetWorld()->GetTimerManager();

		TimerManager.SetTimer(
			StaminaAutoRecoveryTimerHandle,
			this,
			&UStatComponent::StaminaAutoRecoveryTimerPerTick,
			StaminaRecoveryData.TickInterval, //StaminaAutoRecoveryInterval,
			true,
			StaminaRecoveryData.CoolTime //StaminaRecoveryCoolTime
		);

		bResult = true;
	}

	UE_LOG(LogTemp, Log, TEXT("현재 Stamina : %.1f/ %.1f"), CurrentStamina, MaxStamina);
	return bResult;
}

void UStatComponent::RecoveryStamina_Implementation(float InAmount)
{
	CurrentStamina = FMath::Clamp(CurrentStamina + InAmount, 0.0f, MaxStamina);
	UE_LOG(LogTemp, Log, TEXT("현재 Stamina : %.1f/ %.1f"), CurrentStamina, MaxStamina);
	//FMath::IsNearlyEqual(CurrentStamina, MaxStamina);
	if (CurrentStamina >= MaxStamina)
	{
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		TimerManager.ClearTimer(StaminaAutoRecoveryTimerHandle);
	}
}

float UStatComponent::GetCurrentHealth_Implementation() const
{
	return CurrentHealth;
}

void UStatComponent::Damaged_Implementation(float InAmount)
{
	if (CurrentHealth >= InAmount)
	{
		CurrentHealth -= InAmount;
	}

	UE_LOG(LogTemp, Log, TEXT("현재 Health : %.1f/ %.1f"), CurrentHealth, MaxHealth);
}

void UStatComponent::RecoveryHealth_Implementation(float InAmount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + InAmount, 0.0f, MaxHealth);

	UE_LOG(LogTemp, Log, TEXT("현재 Health : %.1f/ %.1f"), CurrentHealth, MaxHealth);
}


// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatComponent::StaminaAutoRecoveryTimerPerTick()
{
	//IStaminaInterface::Execute_RecoveryStamina(this, StaminaAutoRecoveryPerTick);
	IStaminaInterface::Execute_RecoveryStamina(this, StaminaRecoveryData.RecoveryPerTick);

}



////타이머로 대체해서 더 이상 안 씀
//void AActionCharacter::StaminaAutoRecovery(float DeltaTime)
//{
//	StaminaAutoRecoverySecond -= DeltaTime;
//	if (StaminaAutoRecoverySecond < 0.0f)
//	{
//		IStaminaInterface::Execute_RecoveryStamina(this, StaminaAutoRecoveryPerSec * DeltaTime);
//	}
//
//}


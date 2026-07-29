// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PickUpEffect.h"
#include "Component/StatComponent.h"
#include "../Interface/StaminaInterface.h"
#include "../Interface/StatInterface.h"

void APickUpEffect::OnPickup(AActor* InTarget)
{

	UStatComponent* TargetStat = IStatInterface::Execute_GetStatComponent(InTarget);
	if (TargetStat && TargetStat->Implements<UStaminaInterface>())
	{
		if (Stamina > 0)
		{
			IStaminaInterface::Execute_RecoveryStamina(TargetStat, Stamina);
		}
		else
		{
			IStaminaInterface::Execute_ConsumeStamina(TargetStat, -Stamina);
		}
		if (Health > 0)
		{
			IStatInterface::Execute_RecoveryHealth(TargetStat, Health);
		}
		else if (Health < 0)
		{
			IStatInterface::Execute_Damaged(TargetStat, -Health);
		}
	}
}

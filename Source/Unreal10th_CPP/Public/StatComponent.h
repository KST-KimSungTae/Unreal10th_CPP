// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Interface/StaminaInterface.h"
#include "../Interface/StatInterface.h"
#include "StatComponent.generated.h"


struct FAutoRecoveryData
{
	float CoolTime = 3.0f;			//자동 회복이 있을 때 까지 걸릴 시간
	float TickInterval = 0.1f;			//한틱의 시간
	float RecoveryPerTick = 1.0f;	//틱당 회복량

	FAutoRecoveryData() : CoolTime(1.0f), TickInterval(1.0f), RecoveryPerTick(1.0f) {};
	FAutoRecoveryData(float InCoolTime, float InTickInterval, float InRecoveryPerTick)
		: CoolTime(InCoolTime), TickInterval(InTickInterval), RecoveryPerTick(InRecoveryPerTick)
	{
	};
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL10TH_CPP_API UStatComponent : public UActorComponent, public IStaminaInterface, public IStatInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatComponent();

	void InitializeStat(FAutoRecoveryData& InData);
	virtual float GetCurrentStamina_Implementation() const override;

	virtual bool ConsumeStamina_Implementation(float InAmount) override;

	virtual void RecoveryStamina_Implementation(float InAmount) override;

	virtual float GetCurrentHealth_Implementation() const override;

	virtual void Damaged_Implementation(float InAmount) override;

	virtual void RecoveryHealth_Implementation(float InAmount) override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//DEPRECATED : 타이머로 변경하면서 더 이상 사용하지 않음
	//void StaminaAutoRecovery(float DeltaTime);

private:

	void StaminaAutoRecoveryTimerPerTick();

protected:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float CurrentStamina = 100.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float MaxStamina = 100.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float CurrentHealth = 200.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float MaxHealth = 200.0f;

private:
	//DEPRECATED : 틱에서 타이머로 변경되며 사용 안함
	//float StaminaAutoRecoverySecond = 0.0f;

	//스태미너 자동 회복 처리를 위한 타이머
	FTimerHandle StaminaAutoRecoveryTimerHandle;

	//스태미너 자동 회복용 데이터
	FAutoRecoveryData StaminaRecoveryData;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/ActionCharacter.h"
#include "EnemyCharacter.generated.h"

class AWeaponActor;
/**
 * 
 */
UCLASS()
class UNREAL10TH_CPP_API AEnemyCharacter : public AActionCharacter
{
	GENERATED_BODY()
	
public:
	AEnemyCharacter();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

	void AutoAttack();

	// 자동 공격 간격(초)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float AttackInterval = 0.2f;

	// 체력 0이 돼도 리셋해서 계속 때릴 수 있게(무한 샌드백)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	bool bImmortal = true;

private:
	FTimerHandle AutoAttackTimer;
};

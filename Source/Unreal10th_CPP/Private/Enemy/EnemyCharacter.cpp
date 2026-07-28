// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/EnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Unreal10th_CPP/Unreal10th_CPP.h"   // ECC_Enemy, ECC_Weapon
#include "Weapon/WeaponActor.h"
#include "../Interface/StatInterface.h"
#include "Component/StatComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// 플레이어 무기(ECC_Weapon)에 맞으려면 캡슐 ObjectType을 ECC_Enemy로
	// (무기에 대한 Overlap 응답은 부모 ActionCharacter 생성자에서 이미 열어둠)
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Enemy);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();   // 부모: StatComponent 초기화(체력) + AnimInstance 캐싱


	// 자동 공격 반복 (1초 뒤 시작, AttackInterval 간격)
	GetWorldTimerManager().SetTimer(
		AutoAttackTimer, this, &AEnemyCharacter::AutoAttack,
		AttackInterval, /*bLoop=*/true, /*FirstDelay=*/0.2f);
}

void AEnemyCharacter::AutoAttack()
{
	UAnimInstance* Anim = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	if (AttackMontage && Anim && !Anim->IsAnyMontagePlaying())
	{
		PlayAnimMontage(AttackMontage);   // 스태미너 무시하고 바로 스윙
	}
}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	UStatComponent* Stat = IStatInterface::Execute_GetStatComponent(this);
	if (Stat)
	{
		IStatInterface::Execute_Damaged(Stat, DamageAmount);
		UE_LOG(LogTemp, Warning, TEXT("[적] 체력 %.0f / %.0f"),
			IStatInterface::Execute_GetCurrentHealth(Stat),
			IStatInterface::Execute_GetMaxHealth(Stat));

		if (bImmortal && IStatInterface::Execute_GetCurrentHealth(Stat) <= 0.0f)
		{
			IStatInterface::Execute_RecoveryHealth(Stat, IStatInterface::Execute_GetMaxHealth(Stat));
		}
	}
	return DamageAmount;
}
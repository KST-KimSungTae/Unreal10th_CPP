// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "../Interface/StatInterface.h"
#include "Interface/WeaponUserInterface.h"
#include "ActionCharacter.generated.h"


class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UStatComponent;
class UMyAnimNotifyState_SectionJump;
class AWeaponActor;
class UWeaponDataAsset;

UCLASS()
class UNREAL10TH_CPP_API AActionCharacter : public ACharacter, public IStatInterface,public IWeaponUserInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActionCharacter();

	void EquipWeapon_Implementation(UWeaponDataAsset* InWeaponData) override;

	void SetSectionJumpNotify(UMyAnimNotifyState_SectionJump* InSectionJumpNotify);

	virtual void OnWeaponAttackState(bool bEnable) override;

	//Getter & Setter들
	virtual UStatComponent* GetStatComponent_Implementation() const override;

	virtual FOnWeaponAttackStateChanged& GetWeaponAttackStateChangedDelegate() override {
		return OnOnWeaponAttackStateChanged;
	};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
protected:
	void OnTestAction(const FInputActionValue& Value);

	void OnMoveAction(const FInputActionValue& Value);
	void OnAttackAction(const FInputActionValue& Value);

	void OnBoostAction();

	void OnBoostEnd();

	void OnRollAction(const FInputActionValue& Value);

private:
	void SpendSprintStamina(float DeltaTime);

	void SectionJumpForCombo();

public:
	FOnWeaponAttackStateChanged OnOnWeaponAttackStateChanged;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction>IA_Test;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction>IA_Move;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction>IA_Boost;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction>IA_Roll;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction>IA_Attack;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Action Anim")
	TObjectPtr<UAnimMontage> RollMontage;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Action Anim")
	TObjectPtr<UAnimMontage> AttackMontage;



	//구르기에 필요한 스태미너 코스트
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Stat|Stamina")
	float RollStaminaCost = 30.0f;

	//달리기에 필요한 초당 스태미너 코스트
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Stat|Stamina")
	float SprintStaminaCostPerSec = 2.0f;



	//스태미너 사용 후 자동 회복에 걸리는 시간
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Stat|Stamina")
	float StaminaRecoveryCoolTime = 3.0f;

	////DEPRECATED : 스태미너가 자동 회복 될 떄 초당 회복량
	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Stat|Stamina")
	//float StaminaAutoRecoveryPerSec = 10.0f;

	// 스태미너가 자동 회복 될 떄 타이머 틱당 회복량
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Stat|Stamina")
	float StaminaAutoRecoveryPerTick = 1.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Stat|Stamina")
	float StaminaAutoRecoveryInterval = 0.1f;

	//공격시 소비되는 스태미너 양
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Stat|Stamina")
	float AttackCost = 5.0f;

	//현재 장비중인 무기
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	TWeakObjectPtr<AWeaponActor> CurrentWeapon = nullptr;

	TObjectPtr<UWeaponDataAsset>CurrentWeaponData = nullptr;

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> CameraSpringArmComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> CameraComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStatComponent>StatComponent = nullptr;


private:
	UPROPERTY()
	TObjectPtr<UAnimInstance> AnimInstance = nullptr;

	bool bSprintMode = false;

	//발생한 노티파이를 저장해 놓는 변수
	TWeakObjectPtr<UMyAnimNotifyState_SectionJump>SectionJumpNotify = nullptr;
	
	//현재 콤보가 가능한지 확인하기 위한 변수
	bool bComboReady = false;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/WeaponUserInterface.h"
#include "WeaponComponent.generated.h"


class AWeaponActor;
class UWeaponDataAsset;
class UMyAnimNotifyState_SectionJump;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL10TH_CPP_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

	//virtual FOnWeaponAttackStateChanged& GetWeaponAttackStateChangedDelegate() override {
	//	return OnOnWeaponAttackStateChanged;
	//};

	//void SetSectionJumpNotify(UMyAnimNotifyState_SectionJump* InSectionJumpNotify);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

//public:
//	void OnAttackMontageEnded();
//
//private:
//	void SectionJumpForCombo();
//
//	void SpawnWeaponActor();
//
//public:
//	FOnWeaponAttackStateChanged OnOnWeaponAttackStateChanged;
//
//public:
//	//현재 장비중인 무기
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
//	TWeakObjectPtr<AWeaponActor> CurrentWeapon = nullptr;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
//	TObjectPtr<UWeaponDataAsset>CurrentWeaponData = nullptr;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
//	TObjectPtr<UWeaponDataAsset> OriginalWeaponData = nullptr;
//		
//private:
//	//발생한 노티파이를 저장해 놓는 변수
//	TWeakObjectPtr<UMyAnimNotifyState_SectionJump>SectionJumpNotify = nullptr;
//	
//	//현재 콤보가 가능한지 확인하기 위한 변수
//	bool bComboReady = false;
//
//	bool bPendingWeaponSwap = false;
};

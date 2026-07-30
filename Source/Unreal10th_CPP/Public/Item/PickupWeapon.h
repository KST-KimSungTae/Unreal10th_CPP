// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/PickUpBase.h"
#include "Data/WeaponDataAsset.h"
#include "PickupWeapon.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL10TH_CPP_API APickupWeapon : public APickUpBase
{
	GENERATED_BODY()
	
protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void OnPickup(AActor* InTarget) override;

	virtual void OnUpdatePickupEffect();
	virtual void OnFinishPickupEffect();

	bool IsPickEffectAssetReady() const;

protected:
	//이 픽업을 먹었을 때 획득하는 무기 데이터
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Data")
	TObjectPtr<UWeaponDataAsset> WeaponData = nullptr;	  
					
	//아이템을 줍는 연출의 진행 상황용 커브
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect | Pickup")
	TObjectPtr<UCurveFloat>PickUpAlpha;					  
						
	//어아탬울 줍는 연출 중 위 아래로 움직임을 위한 커브
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect | Pickup")
	TObjectPtr<UCurveFloat>PickUpHeight;

	//아이템을 줍는 연출 중 크기 변경을 위한 커브
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect | Pickup")
	TObjectPtr<UCurveFloat>PickUpscale;

	//아이템을 줍는 연출의 전체 진행 시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect | Pickup")
	float PickUpEffectduration = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect | Pickup")
	float PickUpHeightMulti = 50.0f;




private:
	//아이템을 줍는 연출용 타이머 핸들
	FTimerHandle PickupEffectTimerHandle;

	//아이템을 줍는 대상
	TWeakObjectPtr<AActor>TargetActor = nullptr;

	//아이템을 줍는 연출이 진행된 시간
	float PickUpElapsedTime = 0.0f;

	//아이템을 줍는 연출용 타이머의 실행 간격
	const float TimerInterval = 0.02f;

	//아이템을 줍는 위치
	FVector PickupStartLocation;
};

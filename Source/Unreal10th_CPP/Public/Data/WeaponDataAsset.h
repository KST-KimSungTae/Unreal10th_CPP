// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/StreamableManager.h"
#include "WeaponDataAsset.generated.h"

class UStaticMesh;
/**
 * 
 */
UCLASS()
class UNREAL10TH_CPP_API UWeaponDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	void RequestDataLoad(FStreamableDelegate InDelegate);
	bool IsLoadCompleted() const;

public:
	//무기의 메시
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	TSoftObjectPtr<UStaticMesh> Mesh;

	//무기가 Attach될 소켓
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	FName AttachSocketName = TEXT("hand_rSocket");	//hand_rSocket
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	FVector LocationOffset = FVector::ZeroVector;

	//HitArea캡슐의 높이 절반
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitArea")
	float HitAreaHalfHeight = 84.0f;

	//HitArea캡슐의 반지름
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitArea")
	float HitAreaRadius = 34.0f;

	//무기의 공격력
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	float Damage = 20.0f;



protected:

	TSharedPtr<FStreamableHandle> AsyncLoadHandle;
};

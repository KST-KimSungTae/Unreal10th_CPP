// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Unreal10th_CPP/Unreal10th_CPP.h"
#include "WeaponActor.generated.h"

class ACharacter;
class UCapsuleComponent;
UCLASS()
class UNREAL10TH_CPP_API AWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void EquippedToTarget(AActor* InTarget);

	UFUNCTION(BlueprintCallable)
	void InitializeWeapon(UWeaponDataAsset* InData);

	UFUNCTION(BlueprintCallable)
	void DropWeapon();

	void CountSupplies();
protected:

	UFUNCTION(BlueprintCallable)
	void OnEquipped(AActor* InOwner);

	UFUNCTION()
	void OnHitAreaBeginOverlap(
		UPrimitiveComponent*		InOverlappedComponent,
		AActor*						InOtherActor,
		UPrimitiveComponent*		InOtherComp,
		int32						InOtherBodyIndex,
		bool						InbFromSweep,
		const FHitResult&			InSweepResult);

	UFUNCTION(BlueprintCallable)
	void AttackEnable(bool bEnable);


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent>Mesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCapsuleComponent>HitArea = nullptr;

private:
	//무기를 장비하고 있는 대상
	TWeakObjectPtr<ACharacter> OwnerCharacter = nullptr;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite)
	TObjectPtr<UWeaponDataAsset>WeaponData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite)
	int32 CurrentCount;

	//무기 드랍된후 사라질때 까지의 시간
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite)
	float DropLifeSpan = 10.0f;

	//드랍 직후에 플레이어와 물리 상호작용이 안되는 시간
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite)
	float PhysicsDelay = 0.8f;

	// 이 무기가 때릴 대상 채널 (플레이어무기=Enemy, 적무기=Pawn)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TEnumAsByte<ECollisionChannel> TargetChannel = ECC_Enemy;

private:
	//PhysicsDelay 용 핸들
	FTimerHandle PhysicsDelayTimerHandle;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite)
	int32 MaxCount = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite)
	int32 Count = 0;
};

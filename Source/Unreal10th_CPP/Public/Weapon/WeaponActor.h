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
	void OnEquipped(AActor* InOwner);

protected:
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

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName AttachSocketName = TEXT("hand_rSocket");	//hand_rSocket

private:
	//무기를 장비하고 있는 대상
	TWeakObjectPtr<ACharacter> OwnerCharacter = nullptr;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float Damage = 20.0f;


	// 이 무기가 때릴 대상 채널 (플레이어무기=Enemy, 적무기=Pawn)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TEnumAsByte<ECollisionChannel> TargetChannel = ECC_Enemy;
};

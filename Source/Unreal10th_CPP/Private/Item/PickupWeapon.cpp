// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PickupWeapon.h"
#include "Weapon/WeaponActor.h"
#include "Interface/WeaponUserInterface.h"

#include "Components/SphereComponent.h"

void APickupWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (WeaponData)
	{
		if (USkeletalMesh* SkeletalMeshData = WeaponData->Mesh.LoadSynchronous())
		{
			Mesh->SetSkeletalMesh(SkeletalMeshData);
			Mesh->SetRelativeLocation(MeshBaseLocation+WeaponData->LocationOffset);
		}

	}
}

void APickupWeapon::OnPickup(AActor* InTarget)
{
	//타이머가 이미 작동 중이면 종료(중복실행 저지)
	if (GetWorldTimerManager().IsTimerActive(PickupEffectTimerHandle))
	{
		return;
	}
	Super::OnPickup(InTarget);

	/*FActorSpawnParameters SpawnParam;
	SpawnParam.Owner = InTarget;
	SpawnParam.Instigator = InTarget->GetInstigator();

	AWeaponActor* Weapon = GetWorld()->SpawnActor<AWeaponActor>(WeaponType, FTransform::Identity, SpawnParam);
	Weapon->EquippedToTarget(InTarget);*/

	//더이상의 오버랩이 발생하지않게 하기
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TargetActor = InTarget;
	if (IsPickEffectAssetReady())
	{
		PickupStartLocation = Mesh->GetComponentLocation();
		PickUpElapsedTime = 0.0f;
		GetWorldTimerManager().SetTimer(
			PickupEffectTimerHandle,
			this,
			&APickupWeapon::OnUpdatePickupEffect,
			TimerInterval,
			true
		);
	}
	else
	{
		OnFinishPickupEffect();
	}
	
}

void APickupWeapon::OnUpdatePickupEffect()
{
	if (!TargetActor.IsValid())	//타겟이 살아 있을 때만 진행
	{
		OnFinishPickupEffect();
		return;
	}

	PickUpElapsedTime += TimerInterval;
	float Div = FMath::Max(PickUpEffectduration, 0.001f);
	float Progress = PickUpElapsedTime / Div;

	float DistanceAlpha = PickUpAlpha->GetFloatValue(Progress);
	//FVector Start = Mesh->GetComponentLocation();
	FVector Goal = TargetActor.Get()->GetActorLocation();
	FVector NewLocation = FMath::Lerp(PickupStartLocation, Goal, DistanceAlpha);
	Mesh->SetWorldLocation(NewLocation);

	float HeightOffset = PickUpHeight->GetFloatValue(Progress) * PickUpHeightMulti;
	NewLocation.Z += HeightOffset;
	Mesh->SetWorldLocation(NewLocation);
	float Scale = PickUpscale->GetFloatValue(Progress);
	Mesh->SetRelativeScale3D(FVector(Scale));

	if (Progress >= 1.0f)
	{
		OnFinishPickupEffect();
	}
}

void APickupWeapon::OnFinishPickupEffect()
{
	GetWorldTimerManager().ClearTimer(PickupEffectTimerHandle);
	if (TargetActor.IsValid())
	{
		IWeaponUserInterface::Execute_EquipWeapon(TargetActor.Get(), WeaponData);
	}

	Destroy();
}

bool APickupWeapon::IsPickEffectAssetReady() const
{
	return PickUpAlpha!=nullptr && PickUpHeight!=nullptr && PickUpscale!=nullptr;
}


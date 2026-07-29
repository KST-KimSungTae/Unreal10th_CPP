// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PickupWeapon.h"
#include "Weapon/WeaponActor.h"
#include "Interface/WeaponUserInterface.h"

void APickupWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (WeaponData)
	{
		if (UStaticMesh* StaticMeshData = WeaponData->Mesh.LoadSynchronous())
		{
			Mesh->SetStaticMesh(StaticMeshData);
		}

	}
}

void APickupWeapon::OnPickup(AActor* InTarget)
{
	Super::OnPickup(InTarget);

	/*FActorSpawnParameters SpawnParam;
	SpawnParam.Owner = InTarget;
	SpawnParam.Instigator = InTarget->GetInstigator();

	AWeaponActor* Weapon = GetWorld()->SpawnActor<AWeaponActor>(WeaponType, FTransform::Identity, SpawnParam);
	Weapon->EquippedToTarget(InTarget);*/

	IWeaponUserInterface::Execute_EquipWeapon(InTarget, WeaponData);

	Destroy();

	
}

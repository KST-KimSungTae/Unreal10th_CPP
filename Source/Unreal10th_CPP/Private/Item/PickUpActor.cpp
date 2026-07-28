// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PickUpActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Component/StatComponent.h"
#include "../Interface/StaminaInterface.h"
#include "../Interface/StatInterface.h"
#include "Weapon/WeaponActor.h"
#include "Interface/WeaponUserInterface.h"

// Sets default values
APickUpActor::APickUpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("RootCollision"));
	SphereCollision->InitSphereRadius(100.0f);
	SetRootComponent(SphereCollision);


	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SphereCollision);
}

// Called when the game starts or when spawned
void APickUpActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickUpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickUpActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	ApplyEffects(OtherActor);

}

void APickUpActor::ApplyEffects(AActor* InTarget)
{

	// 1) 무기 지급 — WeaponClass가 있고, 무기를 들 수 있는 대상일 때
	if (WeaponClass && Cast<IWeaponUserInterface>(InTarget))
	{
		FActorSpawnParameters Params;
		Params.Owner = InTarget;
		if (AWeaponActor* Weapon = GetWorld()->SpawnActor<AWeaponActor>(
			WeaponClass, GetActorTransform(), Params))
		{
			Weapon->OnEquipped(InTarget);   // 소켓 부착 + AttackEnable 델리게이트 바인딩까지 여기서 처리
			UE_LOG(LogTemp, Log, TEXT("%s 무기 획득"), *InTarget->GetName());
		}
	}

	UStatComponent* TargetStat = IStatInterface::Execute_GetStatComponent(InTarget);
	if (TargetStat && TargetStat->Implements<UStaminaInterface>())
	{
		if (Stamina > 0)
		{
			IStaminaInterface::Execute_RecoveryStamina(TargetStat, Stamina);
		}
		else
		{
			IStaminaInterface::Execute_ConsumeStamina(TargetStat, -Stamina);
		}
		if (Health > 0)
		{
			IStatInterface::Execute_RecoveryHealth(TargetStat, Health);
		}
		else if (Health < 0)
		{
			IStatInterface::Execute_Damaged(TargetStat, -Health);
		}
	}



	//bImplements이 true면 인터페이스를 구현했다.
	//bool bImplements = OtherActor->Implements<UStaminaInterface>()


	/*if (IStatInterface* Stat = Cast<IStatInterface>(InTarget))
	{
		UStatComponent* StatComp = Stat->GetStatComponent();
		if (Stamina > 0)
		{
			IStaminaInterface::Execute_RecoveryStamina(StatComp, Stamina);
		}
		else if(Stamina<0)
		{
			IStaminaInterface::Execute_ConsumeStamina(StatComp, -Stamina);
		}

		if (Health > 0)
		{
			IStatInterface::Execute_RecoveryHealth(StatComp, Health);
		}
		else if (Health < 0)
		{
			IStatInterface::Execute_Damaged(StatComp, -Health);
		}
	}*/




	//Target이 null이 아니면 인터페이스를 상속받았다.(=C++니까 구현도 되어있다. 블루프린트에서 상속을 했을 경우는 체크 불가능)
	//IStaminaInterface* Target = Cast<IStaminaInterface>(OtherActor);
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponActor.h"
#include "Components/CapsuleComponent.h"
#include "GameFrameWork/Character.h"
#include "Unreal10th_CPP/Unreal10th_CPP.h"
#include "Interface/WeaponUserInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Data/WeaponDataAsset.h"
#include "Player/ActionCharacter.h"

// Sets default values
AWeaponActor::AWeaponActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootMesh"));
	SetRootComponent(Mesh);

	//Mesh->SetCollisionProfileName(TEXT("NoCollision"));	//프로파일을 이용해서 한번에 세팅(실제 적용 타이밍은 좀더 뒤)
	
	Mesh->SetGenerateOverlapEvents(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	HitArea = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitArea"));
	HitArea->SetupAttachment(Mesh);

	HitArea->SetCapsuleHalfHeight(84.0f, false);
	HitArea->SetCapsuleRadius(34.0f, false);
	HitArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HitArea->SetCollisionObjectType(ECC_Weapon);
	//HitArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	//HitArea->SetCollisionResponseToChannel(ECC_Enemy, ECR_Overlap);
	HitArea->SetRelativeLocation(FVector(0.0f, 0.0f, 30.0f));

}

// Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();

	// 대상 채널만 Overlap, 나머지는 Ignore
	HitArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	HitArea->SetCollisionResponseToChannel(TargetChannel, ECR_Overlap);


	HitArea->OnComponentBeginOverlap.AddDynamic(this, &AWeaponActor::OnHitAreaBeginOverlap);
}

void AWeaponActor::EquippedToTarget(AActor* InTarget)
{
	OnEquipped(InTarget);
}

void AWeaponActor::InitializeWeapon(UWeaponDataAsset* InData)
{
	WeaponData = InData;
	Mesh->SetStaticMesh(WeaponData->Mesh.Get());
	//Mesh->SetRelativeLocation(WeaponData->LocationOffset);

	HitArea->SetCapsuleHalfHeight(WeaponData->HitAreaHalfHeight);
	HitArea->SetCapsuleRadius(WeaponData->HitAreaRadius);
}

void AWeaponActor::DropWeapon()
{
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	DetachFromActor(DetachRules);

	//Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	//Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	Mesh->SetCollisionProfileName(TEXT("PhysicsActor"));
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Player, ECollisionResponse::ECR_Ignore);
	Mesh->SetSimulatePhysics(true);
	HitArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//일정 시간 동안 무기와 플레이어가 충돌 안하게 설정.
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(
		PhysicsDelayTimerHandle,
		FTimerDelegate::CreateLambda(
			[this]()
			{
				Mesh->SetCollisionResponseToChannel(ECC_Player, ECollisionResponse::ECR_Block);
			}
		),
		PhysicsDelay,
		false
	);

	//뒤로 던지기
	FVector BackwardDirection = -OwnerCharacter->GetActorForwardVector();
	FVector ThrowDirection = BackwardDirection * 300.0f + FVector::UpVector * 200.0f;
	Mesh->AddImpulse(ThrowDirection, NAME_None, true);
	FVector AngularImpulse = FVector(
		FMath::RandRange(-200.0f, 200.0f)
	) + GetActorForwardVector() * 1000.0f;;
	Mesh->AddAngularImpulseInDegrees(AngularImpulse, NAME_None, true);

	SetLifeSpan(DropLifeSpan);

	OwnerCharacter = nullptr;
}

void AWeaponActor::OnEquipped(AActor* InOwner)
{
	if (!WeaponData)
	{
		return;
	}
	SetOwner(InOwner);
	OwnerCharacter = Cast<ACharacter>(InOwner);
	FAttachmentTransformRules AttachRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		true);
	if (OwnerCharacter.IsValid())
	{
		AttachToComponent(OwnerCharacter->GetMesh(), AttachRules, WeaponData->AttachSocketName);

		//offset적용
		SetActorRelativeLocation(WeaponData->LocationOffset);

		HitArea->IgnoreActorWhenMoving(OwnerCharacter.Get(), true);	//만약을 대비한 것

		IWeaponUserInterface* WeaponUser = Cast<IWeaponUserInterface>(OwnerCharacter);
		WeaponUser->GetWeaponAttackStateChangedDelegate().BindUFunction(this, FName("AttackEnable"));
	}
}

void AWeaponActor::OnHitAreaBeginOverlap(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex, bool InbFromSweep, const FHitResult& InSweepResult)
{
	float Damage = WeaponData ? WeaponData->Damage : 1;

	UE_LOG(LogTemp, Log, TEXT("오버랩 된 대상 : %s"), *InOtherActor->GetName());
	UGameplayStatics::ApplyDamage(InOtherActor, Damage, OwnerCharacter->GetController(), this, nullptr);
	//InOtherActor->ReceiveAnyDamage(Damage, );
}

void AWeaponActor::AttackEnable(bool bEnable)
{
	if (bEnable)
	{
		HitArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		HitArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeaponActor::CountSupplies()
{
	if (Count < 3)
	{
		Count++;
	}
	else if (Count >= 3)
	{
		//IWeaponUserInterface::Execute_EquipWeapon(OwnerCharacter.Get(), WeaponData);
		AActionCharacter* ActionOwner = Cast<AActionCharacter>(OwnerCharacter.Get());
		if (ActionOwner)
		{
			ActionOwner->EquipInitialWeapon();
		}
	}
}




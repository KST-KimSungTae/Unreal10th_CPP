// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PickUpBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"

// Sets default values
APickUpBase::APickUpBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("RootCollision"));
	SphereCollision->InitSphereRadius(100.0f);
	SetRootComponent(SphereCollision);



	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SphereCollision);
	Mesh->SetCollisionProfileName("NoCollision");

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VFX"));
	NiagaraComponent->SetupAttachment(SphereCollision);
}

// Called when the game starts or when spawned
void APickUpBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickUpBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIdle)
	{
		OnUpdateUpDownSpin(DeltaTime);
	}
}

void APickUpBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	OnPickup(OtherActor);
}

void APickUpBase::OnPickup(AActor* InTarget)
{
	UE_LOG(LogTemp, Log, TEXT("%s(이)가 %s를 획득했습니다."),
		InTarget ? *InTarget->GetName() : TEXT("알 수 없는 대상"), *this->GetName());
	bIdle = false;
}

void APickUpBase::OnUpdateUpDownSpin(float InDeltaTime)
{
	if (!IsCurveAssetReady()) return;

	ElapsedTime += InDeltaTime;

	float Div = FMath::Max(UpDownDuration, 0.001f);
	float Progress = FMath::Fmod(ElapsedTime / Div, 1.0f);
	FVector NewMeshLocation = MeshBaseLocation;
	NewMeshLocation.Z += UpDownCurve->GetFloatValue(Progress)* UpDownHeight;

	Mesh->SetRelativeLocation(NewMeshLocation);

	float NewAngle = SpinCurve->GetFloatValue(Progress) * 360.0f;
	Mesh->SetRelativeRotation(FRotator(0.0f, NewAngle,0.0f));
}

bool APickUpBase::IsCurveAssetReady() const
{
	return UpDownCurve!=nullptr && SpinCurve!=nullptr;
}


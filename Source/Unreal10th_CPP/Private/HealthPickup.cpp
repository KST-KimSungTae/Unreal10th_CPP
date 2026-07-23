// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "StatComponent.h"
#include "../Interface/StaminaInterface.h"

// Sets default values
AHealthPickup::AHealthPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("RootCollision"));
	SphereCollision->InitSphereRadius(100.0f);
	SetRootComponent(SphereCollision);

	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("Stat"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SphereCollision);
}

// Called when the game starts or when spawned
void AHealthPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHealthPickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	UStatComponent* TargetStat = IStatInterface::Execute_GetStatComponent(OtherActor);
	if (TargetStat && TargetStat->Implements<UStatInterface>())
	{
		if (Damage > 0)
		{
			IStatInterface::Execute_Damaged(TargetStat, Damage);
		}
		else
		{
			IStatInterface::Execute_RecoveryHealth(TargetStat, -Damage);
		}
	}
}

// Called every frame
void AHealthPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


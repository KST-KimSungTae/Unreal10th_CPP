// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthPickup.generated.h"

class USphereComponent;
class UStatComponent;
UCLASS()
class UNREAL10TH_CPP_API AHealthPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealthPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent>SphereCollision = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent>Mesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStatComponent>StatComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 30.0f;
};

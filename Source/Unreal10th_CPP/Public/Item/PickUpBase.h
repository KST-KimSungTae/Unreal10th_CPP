// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpBase.generated.h"

class USphereComponent;
class UNiagaraComponent;
UCLASS()
class UNREAL10TH_CPP_API APickUpBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	//오버랩 됐을 때 대상에게 실제적용되는 함수
	virtual void OnPickup(AActor* InTarget);

	virtual void OnUpdateUpDownSpin(float InDeltaTime);

private:
	bool IsCurveAssetReady() const;

protected:

	// 메시의 기본 위치
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Data")
	FVector MeshBaseLocation = FVector(0.0f, 0.0f, 50.0f);

	//맵에 있을때 모습용 커브
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect | Default")
	TObjectPtr<UCurveFloat>UpDownCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect | Default")
	TObjectPtr<UCurveFloat>SpinCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect | Default")
	float UpDownDuration = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect | Default")
	float UpDownHeight = 100.0f;

	FTimerHandle UpDownSpinTimerhandle;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent>SphereCollision = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent>Mesh = nullptr;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

private:
	float ElapsedTime = 0.0f;
	bool bIdle = true;

};

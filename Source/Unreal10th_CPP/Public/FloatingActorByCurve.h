// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FloatingActor.h"
#include "FloatingActorByCurve.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL10TH_CPP_API AFloatingActorByCurve : public AFloatingActor
{
	GENERATED_BODY()

public:
    AFloatingActorByCurve();

protected:
    virtual void BeginPlay() override;

    // 에디터에서 Curve Float 애셋을 지정
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
    UCurveFloat* MoveCurve;

    // 커브 값에 곱해줄 높이 배율
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
    float Amplitude = 100.f;

private:
    // 시작 위치 (상대 위치 기준)
    FVector StartRelativeLocation;

    // 누적 시간
    float RunningTime = 0.f;

    // 커브의 시간 범위 (반복용)
    float CurveMinTime = 0.f;
    float CurveMaxTime = 0.f;

public:
    virtual void Tick(float DeltaTime) override;
	
};

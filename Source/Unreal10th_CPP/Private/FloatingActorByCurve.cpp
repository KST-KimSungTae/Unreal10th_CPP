// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingActorByCurve.h"


AFloatingActorByCurve::AFloatingActorByCurve()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AFloatingActorByCurve::BeginPlay()
{
    Super::BeginPlay();

    if (MainMesh)
    {
        StartRelativeLocation = MainMesh->GetRelativeLocation();
    }

    if (MoveCurve)
    {
        // 커브가 가진 시간 구간을 얻어온다 (예: 0 ~ 2초)
        MoveCurve->GetTimeRange(CurveMinTime, CurveMaxTime);
        RunningTime = CurveMinTime;
    }
}

void AFloatingActorByCurve::Tick(float DeltaTime)
{
    // 주의: 부모의 Tick(Cos 이동)을 실행하면 안 되므로 Super::Tick 대신
    // AActor::Tick 을 직접 호출한다.
    AActor::Tick(DeltaTime);

    if (!MoveCurve || !MainMesh)
    {
        return;
    }

    RunningTime += DeltaTime;

    // 커브 끝에 도달하면 처음으로 되감기 (반복)
    if (RunningTime > CurveMaxTime)
    {
        RunningTime = CurveMinTime;
    }

    const float CurveValue = MoveCurve->GetFloatValue(RunningTime);

    FVector NewLocation = StartRelativeLocation;
    NewLocation.Z += CurveValue * Amplitude;

    MainMesh->SetRelativeLocation(NewLocation);
}

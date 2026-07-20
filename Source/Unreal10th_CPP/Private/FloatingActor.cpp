// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingActor.h"

// Sets default values
AFloatingActor::AFloatingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//USceneComponent 만들어서 루트로 설정
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	//UStaticMeshComponent 타입으로 디폴트 서브오브젝트 생성하고 MainMesh변수에 저장
	//에디터 창에서 보이는 이름은 "MainMesh"로 설정
	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	MainMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFloatingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFloatingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	Degree++;
	MainMesh->AddRelativeLocation(FVector(0, 0, FMath::Cos(FMath::DegreesToRadians(Degree)) * MoveSpeed * DeltaTime));

}


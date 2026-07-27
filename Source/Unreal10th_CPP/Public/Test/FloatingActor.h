// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingActor.generated.h"

UCLASS()
class UNREAL10TH_CPP_API AFloatingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent>MainMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Degree = 0.0f;

};

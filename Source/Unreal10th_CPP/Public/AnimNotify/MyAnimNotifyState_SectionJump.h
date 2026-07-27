// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "MyAnimNotifyState_SectionJump.generated.h"

class AActionCharacter;

/**
 * 
 */
UCLASS()
class UNREAL10TH_CPP_API UMyAnimNotifyState_SectionJump : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	inline FName GetNextSectionName() const { return NextSectionName; }

	
protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName NextSectionName;

private:
	TWeakObjectPtr<AActionCharacter> OwnerCharacter = nullptr;

};

// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/MyAnimNotifyState_SectionJump.h"
#include "Player/ActionCharacter.h"

void UMyAnimNotifyState_SectionJump::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActionCharacter* OwnerCharacter = Cast<AActionCharacter>(MeshComp->GetOwner());
	if (OwnerCharacter)
	{
		OwnerCharacter->OnAttackMontageEnded();
		OwnerCharacter->SetSectionJumpNotify(this);
	}
}

void UMyAnimNotifyState_SectionJump::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AActionCharacter* OwnerCharacter = Cast<AActionCharacter>(MeshComp->GetOwner());
	if (OwnerCharacter)
	{
		OwnerCharacter->SetSectionJumpNotify(nullptr);
		OwnerCharacter = nullptr;
	}
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}

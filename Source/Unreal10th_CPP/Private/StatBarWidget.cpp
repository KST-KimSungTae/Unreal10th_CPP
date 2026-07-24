// Fill out your copyright notice in the Description page of Project Settings.


#include "StatBarWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UStatBarWidget::TestImageColorChange(FLinearColor Incolor)
{
	TestImage->SetColorAndOpacity(Incolor);
}

void UStatBarWidget::TestTextChange(FText InString)
{
	TestTextBlock->SetText(InString);
}

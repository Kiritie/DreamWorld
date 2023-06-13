// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "DreamWorld.h"

#define LOCTEXT_NAMESPACE "FDreamWorldModule"

void FDreamWorldModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FDreamWorldModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, DreamWorld, "DreamWorld" );

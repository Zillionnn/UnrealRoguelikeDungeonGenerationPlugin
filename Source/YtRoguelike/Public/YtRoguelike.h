// Copyright Yntoo, 2024
// All Rights Reserved.
#pragma once

#include "Modules/ModuleManager.h"

class FYtRoguelikeModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

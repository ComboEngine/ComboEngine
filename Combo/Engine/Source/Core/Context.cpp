#include "pch.h"
#include "Context.h"
#ifdef COMBO_DIRECTX11
#include <Core/DirectX11/ContextDX11.h>
#endif

void Context::Create(Context** context, ContextSpecification Specification)
{
	//Select window api
#ifdef COMBO_DIRECTX11
	*context = new ContextDX11();
#endif
	Context* contextPtr = *context;
	contextPtr->Init();

	LOG(std::string("Created ") + contextPtr->GetApiName() + " Context");
}

#include "pch.h"
#include "Context.h"
#ifdef COMBO_DIRECTX11
#include <Core/DirectX11/ContextDX11.h>
#endif

void Context::Create(Scope<Context>& context, ContextSpecification Specification)
{
	context = Scope<Context>();
	//Select window api
#ifdef COMBO_DIRECTX11
	context.Set(new ContextDX11());
#endif
	context.Get()->Init();

	LOG(std::string("Created ") + context.Get()->GetApiName() + " Context");
}

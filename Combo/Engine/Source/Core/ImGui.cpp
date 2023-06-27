#include "pch.h"
#include "ImGui.h"
#include "Core.h"


#ifdef COMBO_DIRECTX11
#include "DirectX11/ContextDX11.h"
#include "DirectX11/WindowDX11.h"
#endif
#ifdef COMBO_VULKAN
#include "Vulkan/ContextVulkan.h"
#include "Vulkan/WindowVulkan.h"
#endif
void ImGuiAdapter::Init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

	ImGui::GetIO().Fonts->AddFontFromFileTTF("./Content/Roboto.ttf", 15.0f);
#ifdef COMBO_DIRECTX11
    ContextDX11* context = reinterpret_cast<ContextDX11*>(Core::s_Context);
    ImGui_ImplGlfw_InitForOther(reinterpret_cast<WindowDX11*>(Core::s_Window)->glfwWindow, true);
    ImGui_ImplDX11_Init(context->Device, context->Context);
#endif
#ifdef COMBO_VULKAN
	ContextVulkan* context = ContextVulkan::GetVulkanContext();
	VkDescriptorPoolSize pool_sizes[] =
	{
		{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
	};

	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	pool_info.maxSets = 1000;
	pool_info.poolSizeCount = std::size(pool_sizes);
	pool_info.pPoolSizes = pool_sizes;

	VkDescriptorPool imguiPool;
	CB_CHECKHR(vkCreateDescriptorPool(context->Device, &pool_info, nullptr, &imguiPool));

	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = context->Instance;
	init_info.PhysicalDevice = context->PhysicalDevice;
	init_info.Device = context->Device;
	init_info.Queue = context->GetRenderCmdBuffer()->GraphicsQueue;
	init_info.DescriptorPool = imguiPool;
	init_info.MinImageCount = 3;
	init_info.ImageCount = 3;
	init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

	ImGui_ImplGlfw_InitForVulkan(reinterpret_cast<WindowVulkan*>(Core::s_Window)->glfwWindow, true);
	ImGui_ImplVulkan_Init(&init_info, context->GetSwapChain()->RenderPass);

	context->SubmitBarrier([&](VkCommandBuffer cmd) {
		ImGui_ImplVulkan_CreateFontsTexture(cmd);
	});

	ImGui_ImplVulkan_DestroyFontUploadObjects();

#endif

}

void ImGuiAdapter::StartFrame()
{
#ifdef COMBO_DIRECTX11
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplGlfw_NewFrame();
#endif
#ifdef COMBO_VULKAN
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
#endif
    ImGui::NewFrame();
}

void ImGuiAdapter::EndFrame()
{
    ImGui::Render();
#ifdef COMBO_DIRECTX11
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif
#ifdef COMBO_VULKAN
	ContextVulkan::GetVulkanContext()->SubmitBarrierAtEnd([&](VkCommandBuffer cmd) {
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);
	});
#endif
}

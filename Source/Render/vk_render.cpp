
#include <vulkan/vulkan.h>
#include <iostream>
#include <vulkan/vulkan_win32.h>

#define ArraySize(arr) sizeof((arr))/ sizeof((arr[0]))

#define VK_CHECK(result)                                \
if(result != VK_SUCCESS){                               \
     std::cout <<"Vulkan error "<< result << '\n'; \
   __debugbreak();                                      \
   return false;                                        \
}


struct VkContext{

 VkInstance instance;
 VkSurfaceKHR surface;
};


bool vk_init(VkContext* vkcontext, HWND *window){

   VkApplicationInfo appinfo = {};
   appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
   appinfo.pApplicationName = "ping";
   appinfo.pEngineName = "pingEngine";

   char* extensions[] = {VK_KHR_SURFACE_EXTENSION_NAME,VK_KHR_WIN32_SURFACE_EXTENSION_NAME};
    
   VkInstanceCreateInfo InstanceInfo = {};
   InstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
   InstanceInfo.pApplicationInfo = &appinfo;
   InstanceInfo.ppEnabledExtensionNames = extensions;
   InstanceInfo.enabledExtensionCount = ArraySize(extensions);
   VK_CHECK(vkCreateInstance(&InstanceInfo, 0, &vkcontext->instance));    
   
   VkWin32SurfaceCreateInfoKHR surfaceinfo = {};
   surfaceinfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
   surfaceinfo.pNext = NULL;
   surfaceinfo.flags = 0;
   surfaceinfo.hinstance = GetModuleHandleA(0);
   surfaceinfo.hwnd = (HWND)window; 
   VK_CHECK(vkCreateWin32SurfaceKHR(vkcontext->instance, &surfaceinfo, 0, &vkcontext->surface));

   return false;
}


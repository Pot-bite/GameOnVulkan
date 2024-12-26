
#include <vulkan/vulkan.h>
#include <iostream>
#ifdef WINDOWS_BUILD
#include <vulkan/vulkan_win32.h>
#elif LINUX_BUILD
#endif

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
 VkPhysicalDevice gpu;

 int graphicsidx;
};


bool vk_init(VkContext* vkcontext, HWND *window){

   VkApplicationInfo appinfo = {};
   appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
   appinfo.pApplicationName = "ping";
   appinfo.pEngineName = "pingEngine";

   char* extensions[] = { 
    #ifdef WINDOWS_BUILD 
    VK_KHR_SURFACE_EXTENSION_NAME,
    #elif LINUX_BUILD
    #endif
    VK_KHR_WIN32_SURFACE_EXTENSION_NAME
   };
    
   VkInstanceCreateInfo InstanceInfo = {};
   InstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
   InstanceInfo.pApplicationInfo = &appinfo;
   InstanceInfo.ppEnabledExtensionNames = extensions;
   InstanceInfo.enabledExtensionCount = ArraySize(extensions);
   VK_CHECK(vkCreateInstance(&InstanceInfo, 0, &vkcontext->instance));    
   
   {
    #ifdef WINDOWS_BUILD
    VkWin32SurfaceCreateInfoKHR surfaceinfo = {};
    surfaceinfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceinfo.pNext = NULL;
    surfaceinfo.flags = 0;
    surfaceinfo.hinstance = GetModuleHandleA(0);
    surfaceinfo.hwnd = (HWND)window; 
    VK_CHECK(vkCreateWin32SurfaceKHR(vkcontext->instance, &surfaceinfo, 0, &vkcontext->surface));
    #elif LINUX_BUILD
    #endif
   }


    {
      vkcontext->graphicsidx = -1;
      uint32_t gpuCount = 0;
      //TODO: Suballocation from main Allocation

      VkPhysicalDevice gpus[10];
      VK_CHECK(vkEnumeratePhysicalDevices(vkcontext->instance, &gpuCount, 0));
      VK_CHECK(vkEnumeratePhysicalDevices(vkcontext->instance, &gpuCount, gpus));


      for(uint32_t i = 0; i < gpuCount; i++){
         VkPhysicalDevice gpu = gpus[i];
         uint32_t QueueFamilyCount = 0;
         //TODO: Suballocation from main Allocation
         VkQueueFamilyProperties queueProps[10];
         vkGetPhysicalDeviceQueueFamilyProperties(gpu, &QueueFamilyCount, 0);
         vkGetPhysicalDeviceQueueFamilyProperties(gpu, &QueueFamilyCount, queueProps);

         for(uint32_t j = 0; j < QueueFamilyCount; j++){

           if(queueProps[j].queueFlags & VK_QUEUE_GRAPHICS_BIT){
            VkBool32 SurfaceSupport = VK_FALSE;
            VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(gpu, j, vkcontext->surface, &SurfaceSupport));
               if(SurfaceSupport){
            vkcontext->graphicsidx = j;
            vkcontext->gpu = gpu;
             }

          }

            


        }

    }

      if(vkcontext->graphicsidx < 0){
        return false;
      }

  }

    return false;
}


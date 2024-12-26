
#include <vulkan/vulkan.h>
#include <iostream>
#ifdef WINDOWS_BUILD
#include <vulkan/vulkan_win32.h>
#elif LINUX_BUILD
#endif

#define ArraySize(arr) sizeof((arr))/ sizeof((arr[0]))

#define VK_CHECK(result)                                \
if(result != VK_SUCCESS){                               \
     std::cout <<"Vulkan error "<< result << '\n';      \
   __debugbreak();                                      \
   return false;                                        \
}

static VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(
  VkDebugUtilsMessageSeverityFlagBitsEXT msgSeverity,
  VkDebugUtilsMessageTypeFlagsEXT msgFlags,
  const VkDebugUtilsMessengerCallbackDataEXT *pCallBackData,
  void *pUserData
)
{
  std::cout << "Vallidation Error: " << pCallBackData->pMessage << '\n';
  return false;
}

struct VkContext{

 VkInstance instance;
 VkSurfaceKHR surface;
 VkPhysicalDevice gpu;
 VkDevice device;
 VkSwapchainKHR swapchain;
 VkDebugUtilsMessengerEXT debugMessenger;

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
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
    VK_KHR_WIN32_SURFACE_EXTENSION_NAME
   };
    

   char*layers[]={"VK_LAYER_KHRONOS_validation"};  

   VkInstanceCreateInfo InstanceInfo = {};
   InstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
   InstanceInfo.pApplicationInfo = &appinfo;
   InstanceInfo.ppEnabledExtensionNames = extensions;
   InstanceInfo.enabledExtensionCount = ArraySize(extensions);
   InstanceInfo.ppEnabledLayerNames = layers;
   InstanceInfo.enabledLayerCount = ArraySize(layers);
   VK_CHECK(vkCreateInstance(&InstanceInfo, 0, &vkcontext->instance));    
   

   auto vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vkcontext->instance,"vkCreateDebugUtilsMessengerEXT");

   if(vkCreateDebugUtilsMessengerEXT)
   {
    VkDebugUtilsMessengerCreateInfoEXT debuginfo = {};
    debuginfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debuginfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
    debuginfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    debuginfo.pfnUserCallback = vk_debug_callback;

    vkCreateDebugUtilsMessengerEXT(vkcontext->instance,&debuginfo,0,&vkcontext->debugMessenger);

   }
   else
   {
     return false;
   }


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

     // GPU Device
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



   // Logical Device
   {
    float queuepriorities = 1.0f;
    VkDeviceQueueCreateInfo queueinfo = {};
    queueinfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueinfo.queueFamilyIndex = vkcontext->graphicsidx;
    queueinfo.pQueuePriorities = &queuepriorities;
    queueinfo.queueCount = 1;

    char* extensions[]={VK_KHR_SWAPCHAIN_EXTENSION_NAME}; 

    VkDeviceCreateInfo deviceinfo = {};
    deviceinfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceinfo.pQueueCreateInfos = &queueinfo;
    deviceinfo.queueCreateInfoCount = 1;
    deviceinfo.ppEnabledExtensionNames = extensions;
    deviceinfo.enabledExtensionCount = ArraySize(extensions);
    
    VK_CHECK(vkCreateDevice(vkcontext->gpu, &deviceinfo, 0, &vkcontext->device));

   }

   // Swapchain
   {

      VkSwapchainCreateInfoKHR scinfo = {};
      scinfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
      scinfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
      scinfo.surface = vkcontext->surface;

      VK_CHECK(vkCreateSwapchainKHR(vkcontext->device, &scinfo, 0, &vkcontext->swapchain));
 
   }



    return false;
}


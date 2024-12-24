
#include <vulkan/vulkan.h>



struct VkContext{

 VkInstance instance;

};


bool vk_init(VkContext* vkcontext){

   VkApplicationInfo appinfo = {};
   appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
   appinfo.pApplicationName = "ping";
   appinfo.pEngineName = "pingEngine";
    
   VkInstanceCreateInfo InstanceInfo = {};
   InstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
   InstanceInfo.pApplicationInfo = &appinfo;



   VkResult result = vkCreateInstance(&InstanceInfo, 0, &vkcontext->instance);    

  if(result == VK_SUCCESS)  return true;

  return false;
}


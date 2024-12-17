
#include <vulkan/vulkan.h>
#include <iostream>


int main(){

   VkApplicationInfo appinfo = {};
   appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
   appinfo.pApplicationName = "ping";
   appinfo.pEngineName = "pingEngine";
    
   VkInstanceCreateInfo InstanceInfo = {};
   InstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
   InstanceInfo.pApplicationInfo = &appinfo;

   VkInstance instance;

   VkResult result = vkCreateInstance(&InstanceInfo, 0, &instance);    

   if(result == VK_SUCCESS){
     std::cout << "vulkan instance created" << '\n';
   }
   

   
   

    return 0;
}
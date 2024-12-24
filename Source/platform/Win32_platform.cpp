#include <windows.h>
#include "Render/vk_render.cpp"


static bool running = true; 


LRESULT CALLBACK platform_window_callback(HWND window, UINT msg, WPARAM wparam, LPARAM lparam){
   switch(msg){
      case WM_CLOSE:
      running = false;
      break;
    }
    return DefWindowProcA(window, msg, wparam, lparam);
}





bool platform_create_windowW(HWND window){

   
    HINSTANCE Instance = GetModuleHandleA(0);

    

    WNDCLASS wc = {0};

    wc.lpfnWndProc = platform_window_callback;
    wc.hInstance = Instance;
    wc.lpszClassName = TEXT("vulkan_engine_class");
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);    
    

    


    if(!RegisterClass(&wc)){
        MessageBoxA(window, "Failed redistering window class", "Error", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    
     window = CreateWindowExA(WS_EX_APPWINDOW,"vulkan_engine_class","Pong",WS_THICKFRAME|WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_OVERLAPPED,100, 100, 800, 720, 0, 0, Instance,0);
    
      if(window == 0){
        MessageBoxA(window, "Could not find a window", "Error", MB_ICONEXCLAMATION | MB_OK);
        return false;
     }

    
    ShowWindow(window,SW_SHOW);
    
    return true;
}


void platform_update_window(HWND window){

 MSG msg;

 while(PeekMessageA(&msg, window, 0,0,PM_REMOVE)){
   TranslateMessage(&msg);
   DispatchMessageA(&msg);
 }

}


int main(){
    
   HWND windowW = 0; 
   VkContext vkcontext = {};

   if(!platform_create_windowW(windowW)){
      return -1;
   }

   if(!vk_init(&vkcontext)){
    return -1;
   }

   while(running){
     platform_update_window(windowW);
   };


  
    return 0;
}
#include <windows.h>


static bool running = true; 

LRESULT CALLBACK platform_window_callback(HWND window, UINT msg, WPARAM wparam, LPARAM lparam){


 switch(msg){
    case WM_CLOSE:
    running = false;
    break;
 }

  return DefWindowProcA(window, msg, wparam, lparam);

}



bool paltform_create_window(HWND window){

    HINSTANCE Instance = GetModuleHandleA(0);

    WNDCLASS wc = {};

    wc.lpfnWndProc = platform_window_callback;
    wc.hInstance = Instance;
    wc.lpszClassName = L"vulkan_engine";

    if(!RegisterClass(&wc)){
        MessageBoxA(window, "Failed redistering window class", "Error", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    
    window = CreateWindowExA(
        WS_EX_APPWINDOW,
        "vulkan_engine_class",
        "Pong",
        0, 100, 100, 1600, 720, 0, 0, Instance,0);
    
    if(window == 0){
        MessageBoxA(window, "Could not find a window", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(window, SW_SHOW);
    
}


int main(){

    HWND window = 0;

   if(paltform_create_window(window)){

    return -1;

   }





    return 0;
}
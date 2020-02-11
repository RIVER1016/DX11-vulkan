# DX11-vulkan
* 实现vulkanMain.cpp分解，提取多个hpp-cpp组合文件包括struct结构成员变量和成员函数(eg: vkDevice、vkSwapchain...

* 组合成员变量和成员函数构成类对象
(eg:
```c++
class Device
{
private:
    vkDeivce device;
    ...
public:
    void createDevice( ... );
}
```
* 根据实现功能进一步封装，达到DX11 API 项目效果

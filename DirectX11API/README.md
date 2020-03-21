【问题】

* Effects11框架链接lib，问题多多，一直报错

【总结】

#### 完成一个黑屏的窗口

* 包括Window, Renderer

![image-201](https://user-images.githubusercontent.com/37433487/77224636-8a74b600-6ba2-11ea-9eab-f157b5f20e57.png)

#### 完成一个三角形的渲染

* 一个简单地渲染管线流程

  ![image-2020](https://user-images.githubusercontent.com/37433487/77224637-8b0d4c80-6ba2-11ea-9171-e0100422aa3d.png)
  
  

#### 完成一个带光照的旋转正方体 

* 对三角形，shader，光照模型的初步探索

  ![image-20200102182926530](https://user-images.githubusercontent.com/37433487/77224638-8d6fa680-6ba2-11ea-8605-d3d78d575929.png)
  
  

#### D3D11的渲染管线

1. 创建顶点Vertex结构体，包含顶点的各种信息（位置，颜色，法向量 ）
2. 建立一个VertexBuffer，存储顶点类型数组，并通过ID3D11DeviceContext::IASetVertexBuffers方法绑定到Input Slot
3. 初始化VertexShader，这里使用effects11框架，如下fx文件：

```c++
struct VertexIn
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float4 Color : COLOR;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float4 Normal : NORMAL;
	float4 Color : COLOR;
};

VertexOut VS( VertexIn vin )
{
	VertexOut vout;
	vout.PosH = mul( float4( vin.Pos, 1.f ), gWorld );
	vout.PosH = mul( vout.PosH, gView );
	vout.PosH = mul( vout.PosH, gProjection );
	vout.Color = vin.Color;
	vout.Normal =  mul( float4( vin.Normal, 1.f ), gWorld );
	return vout;
};
```

4. 在shader的过程中，需要关联一个costant buffer提供world、view、projection矩阵，来将顶点初始3D位置转换到屏幕2D位置上，实现WVP矩阵变换。

5. 这里的Vertex buffer并没有明确的定义是作为点、线或者三角形来使用，所以要通过Primitive Topology来告诉D3D如何"看待"这一串顶点：

```c++
g_pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
```

D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST是枚举类型中的参数，表示这串顶点作为三角形顶点看，如下图：

![image-20200102193519981](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20200102193519981.png)

6. 光栅化渲染状态设置，使用光栅化渲染一个三角形：

   ```c++
   void CD3D11_RASTERIZER_DESC(
     D3D11_FILL_MODE fillMode,
     D3D11_CULL_MODE cullMode,
     BOOL            frontCounterClockwise,
     INT             depthBias,
     FLOAT           depthBiasClamp,
     FLOAT           slopeScaledDepthBias,
     BOOL            depthClipEnable,
     BOOL            scissorEnable,
     BOOL            multisampleEnable,
     BOOL            antialiasedLineEnable
   );
   
   HRESULT CreateRasterizerState(
     const D3D11_RASTERIZER_DESC *pRasterizerDesc,
     ID3D11RasterizerState       **ppRasterizerState
   );
   ```

   fillMode：控制三角形填充模式：填充/线框

   cullMode：背面剔除

   depthClipEnable：裁剪

7. 初始化PixelShader，相比于VS是对于顶点位置的操作，PS就是对顶点颜色的操作，整合光、雾等等各种视觉影响效果，并把最终合成的颜色显示在屏幕上

8. 通过混合之后，直接在Render函数中调用Draw方法，就能够在屏幕上画出想要的三角形，但这里我们只能够按照Vertex Buffer中参数的顺序，每3个一组画出一个三角形，如果要组成一个正方体很麻烦，在这里就引入了Index Buffer，根据顶点在Vertex Buffer里的索引值，重复使用顶点构造新的三角形，组成一整个正方体。

#### 结构体跨块访问

![image-21224](https://user-images.githubusercontent.com/37433487/77225672-222ad200-6bac-11ea-8e3d-217a2c496e9c.png)

问题：颜色传进去白色，显示出来黄色

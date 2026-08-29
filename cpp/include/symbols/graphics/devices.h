// #pragma once
// #include <macros>

// class ITexture {
// public:
//     virtual void* GetNativeHandle() = 0;
//     virtual void Release() {}
//     virtual ~ITexture() = default;
// };

// class IShader {
// public:
//     virtual void* GetNativeHandle() = 0;
//     virtual void Release() {}
//     virtual ~IShader() = default;
// };

// class IInputLayout {
// public:
//     virtual void* GetNativeHandle() = 0;
//     virtual void Release() {}
//     virtual ~IInputLayout() = default;
// };


// class ISamplerState {
// public:
//     virtual void* GetNativeHandle() = 0;
//     virtual void Release() {}
// };

// class IViewPort {
// public:
//     virtual void* GetNativeHandle() = 0;
//     virtual void Release() {}
// };

// enum class TextureFilter
// {
//     Nearest,
//     Linear,
//     Anisotropic
// };

// enum class TextureAddressMode
// {
//     Wrap,
//     Mirror,
//     Clamp,
//     Border
// };

// enum class ComparisonFunc
// {
//     Never,
//     Less,
//     Equal,
//     LessEqual,
//     Greater,
//     NotEqual,
//     GreaterEqual,
//     Always
// };

// struct SamplerStateDesc
// {
//     TextureFilter       minFilter = TextureFilter::Linear;   // Minification
//     TextureFilter       magFilter = TextureFilter::Linear;   // Magnification
//     TextureFilter       mipFilter = TextureFilter::Linear;   // Mipmap filtering

//     TextureAddressMode  addressU = TextureAddressMode::Wrap; // U axis wrap mode
//     TextureAddressMode  addressV = TextureAddressMode::Wrap; // V axis wrap mode
//     TextureAddressMode  addressW = TextureAddressMode::Wrap; // W axis wrap mode

//     float               mipLODBias = 0.0f;  // Mipmap LOD bias
//     u32                 maxAnisotropy = 1;  // For anisotropic filtering

//     ComparisonFunc      comparisonFunc = ComparisonFunc::Always; // For depth comparison samplers

//     float               borderColor[4] = { 0, 0, 0, 0 }; // RGBA for border mode

//     float               minLOD = 0.0f;  // Minimum LOD clamp
//     float               maxLOD = FLOATMAX; // Maximum LOD clamp
// };


// enum class BufferType {
//     Vertex,
//     Index,
//     Constant,
//     // Add more as needed
// };

// class IBuffer {
// public:
//     virtual void* GetNativeHandle() = 0;
//     virtual void Release() {}
//     virtual ~IBuffer() = default;
// };

// enum class TextureFormat {
//     Unknown,

//     // 8-bit per channel formats
//     RGBA8_UNorm,
//     RGBA8_UNorm_SRGB,
//     BGRA8_UNorm,
//     BGRA8_UNorm_SRGB,

//     // 16-bit float formats
//     RGBA16_Float,
//     R16_Float,
//     R16_UNorm,

//     // 32-bit float formats
//     R32_Float,
//     RG32_Float,
//     RGBA32_Float,

//     // Depth/Stencil formats
//     D24_UNorm_S8_ui32,
//     D32_Float,
//     D32_Float_S8X24_ui32,

// };


// struct TextureDesc {
//     u32 width;
//     u32 height;
//     u32 widthBytes;
//     u32 mipLevels;
//     bool isRenderTarget;
//     bool isDepthStencil;
//     bool CpuAccessWrite;
//     bool Dynamic;
//     TextureFormat format;
//     const char* debugName;
//     int slot;
// };

// struct ShaderDesc {
//     enum class Type { Vertex, Pixel, Compute, Geometry };
//     ShaderDesc::Type type;
//     u8* bytecode;
//     long bytecodeSize;
//     const char* debugName = "";
// };

// struct BufferDesc {
//     u64 size;
//     u8 stride;
//     bool dynamic;
//     BufferType type;
//     const char* debugName = "";
// };

// enum class PrimitiveType {
//     Triangles,
//     TriangleStrip,
//     Lines,
//     LineStrip,
//     Points,
//     Quads
// };

// struct ViewPortDesc {
//     void* viewport;
// };

// struct ViewPort {
//     long x;
//     long y;
//     long width;
//     long height;
//     long minDepth;
//     long maxDepth;
// };

// class IDevice {
// public:
//     virtual void* GetNativeHandle() = 0;
//     virtual void Release() {}
// };

// class IDeviceContext {
// public:
//     virtual void* GetNativeHandle() = 0;
//     virtual void Release() {}
// };

// class IRasterizerState {
// public:
//     virtual void* GetNativeHandle() = 0;
//     virtual void Release() {}
// };

// class IDepthStencilState {
// public:
//     virtual void* GetNativeHandle() = 0;
//     virtual void Release() {}
// };

// class IBlendState {
// public:
//     virtual void* GetNativeHandle() = 0;
//     virtual void Release() {}
// };

// enum class IFillMode {
//     Solid,
//     Wireframe
// };

// enum class CullMode {
//     None,
//     Front,
//     Back
// };

// struct RasterizerDesc {
//     IFillMode fillMode = IFillMode::Solid;
//     CullMode cullMode = CullMode::Back;
//     bool frontCounterClockwise = false;

//     int depthBias = 0;
//     float depthBiasClamp = 0.0f;
//     float slopeScaledDepthBias = 0.0f;

//     bool depthClipEnable = true;
//     bool scissorEnable = false;
//     bool multisampleEnable = false;
//     bool antialiasedLineEnable = false;
// };

// enum class StencilOp {
//     Keep,
//     Zero,
//     Replace,
//     IncrSat,
//     DecrSat,
//     Invert,
//     Incr,
//     Decr
// };

// struct StencilOpDesc {
//     StencilOp failOp = StencilOp::Keep;
//     StencilOp depthFailOp = StencilOp::Keep;
//     StencilOp passOp = StencilOp::Keep;
//     ComparisonFunc func = ComparisonFunc::Always;
// };

// struct DepthStencilDesc {
//     bool depthEnable = true;
//     bool depthWriteMask = true;  // true = ALL, false = ZERO
//     ComparisonFunc depthFunc = ComparisonFunc::Less;

//     bool stencilEnable = false;
//     u8 stencilReadMask = 0xFF;
//     u8 stencilWriteMask = 0xFF;

//     StencilOpDesc frontFace; // operations if front-facing
//     StencilOpDesc backFace;  // operations if back-facing
// };

// enum class Blend {
//     Zero,
//     One,
//     SrcColor,
//     InvSrcColor,
//     SrcAlpha,
//     InvSrcAlpha,
//     DestAlpha,
//     InvDestAlpha,
//     DestColor,
//     InvDestColor,
//     SrcAlphaSat,
//     BlendFactor,
//     InvBlendFactor,
//     Src1Color,
//     InvSrc1Color,
//     Src1Alpha,
//     InvSrc1Alpha
// };

// enum class BlendOp {
//     Add,
//     Subtract,
//     RevSubtract,
//     Min,
//     Max
// };

// struct RenderTargetBlendDesc {
//     bool blendEnable = false;

//     Blend srcBlend = Blend::One;
//     Blend destBlend = Blend::Zero;
//     BlendOp blendOp = BlendOp::Add;

//     Blend srcBlendAlpha = Blend::One;
//     Blend destBlendAlpha = Blend::Zero;
//     BlendOp blendOpAlpha = BlendOp::Add;

//     u8 renderTargetWriteMask = 0x0F; // RGBA
// };

// struct BlendStateDesc {
//     bool alphaToCoverageEnable = false;
//     bool independentBlendEnable = false;

//     RenderTargetBlendDesc renderTarget[8]; // up to 8 RTs
// };

// class IRenderTarget {
// public:
//     virtual void* GetNativeHandle() = 0;
//     virtual void Release() {}
// };

// class IDepthStencil {
// public:
//     virtual void* GetNativeHandle() = 0;
//     virtual void Release() {}
// };

// enum class VertexAttribType {
//     Float,
//     Float2,
//     Float3,
//     Float4,
//     ui32,
//     ui322,
//     ui323,
//     ui324,
//     // Add more as needed
// };

// enum class VertexAttribUsage {
//     Position,
//     Normal,
//     TexCoord,
//     Color,
//     Tangent,
//     Bitangent,
//     // Add more as needed
// };

// struct VertexAttribDesc {
//     const char* semanticName;       // e.g., "TEXCOORD", "POSITION"
//     u32 semanticIndex = 0;     // for TEXCOORD0, TEXCOORD1 etc
//     VertexAttribType type;          // Float3, Float2, etc
//     u32 inputSlot = 0;         // which vertex buffer slot
//     u64 offset = 0;              // offsetof in struct
//     bool perInstance = false;       // true = per-instance data
// };

// struct InputLayoutDesc {
//     VertexAttribDesc* elements;
//     long size;
//     void* vertexShader;
//     u32 stride = 0;
// };


// enum GraphicsDeviceFlags : u8 {
//     IS_EXPLICIT = 1 << 0, // 0000 0001
//     IS_IMMEDIATE = 1 << 1, // 0000 0010
// };


// class IGraphicsDevice {
// public:
//     virtual const char* GetAPIName() const = 0;
//     virtual u32 GetAPIVersion() const = 0;
//     virtual bool IsFeatureSupported(const char* feature) const = 0;
//     virtual u8 GetFlags() const = 0;
// };

// class IGraphicsDeviceImmediate : IGraphicsDevice {
// public:
//     virtual const char* GetAPIName() const = 0;
//     virtual u32 GetAPIVersion() const = 0; 
//     virtual bool IsFeatureSupported(const char* feature) const = 0;
//     virtual u8 GetFlags() const {
//         return GraphicsDeviceFlags::IS_IMMEDIATE;
//     }

//     virtual bool Init() = 0;
//     virtual void BeginFrame() = 0;
//     virtual void EndFrame() = 0;
//     virtual void Present(bool vsync = true) = 0;
//     virtual void Resize(long width, long size) = 0;

//     virtual ITexture* CreateTexture(const TextureDesc& desc, const void* initialData = nullptr) = 0;
//     virtual IShader* CreateShader(const ShaderDesc& desc) = 0;
//     virtual IBuffer* CreateBuffer(const BufferDesc& desc, const void* data = nullptr) = 0;
//     virtual IInputLayout* CreateInputLayout(const InputLayoutDesc& desc) = 0;
//     virtual ISamplerState* CreateSamplerState(const SamplerStateDesc& desc) = 0;
//     virtual IViewPort* CreateViewPort(const ViewPortDesc& desc) = 0;
//     virtual IRasterizerState* CreateRasterizerState(const RasterizerDesc& desc) = 0;
//     virtual IDepthStencilState* CreateDepthStencilState(const DepthStencilDesc& desc) = 0;
//     virtual IBlendState* CreateBlendState(const BlendStateDesc& desc) = 0;
//     virtual IRenderTarget* CreateRenderTarget(ITexture* texture) = 0;
//     virtual IDepthStencil* CreateDepthStencil(ITexture* texture) = 0;

//     virtual void ReleaseShader(IShader*) = 0;
//     virtual void ReleaseBuffer(IBuffer*) = 0;
//     virtual void ReleaseTexture(ITexture*) = 0;

//     virtual void BindShader(IShader*) = 0;
//     virtual void BindBuffer(IBuffer*) = 0;
//     virtual void BindTexture(ITexture*) = 0;
//     virtual void BindInputLayout(IInputLayout*) = 0;
//     virtual void BindSamplerState(ISamplerState*) = 0;
//     virtual void BindViewPort(IViewPort*) = 0;
//     virtual void BindRasterizerState(IRasterizerState*) = 0;
//     virtual void BindDepthStencilState(IDepthStencilState*, ui32 stencilRef = 0) = 0;
//     virtual void BindBlendState(IBlendState*, const float blendFactor[4], ui32 sampleMask = 0xFFFFFFFF) = 0;
//     virtual void BindRenderTarget(IRenderTarget*, IDepthStencil* = nullptr) = 0;
//     virtual void ClearRenderTarget(IRenderTarget* target, const float color[4]) = 0;
//     virtual void ClearDepthStencil(IDepthStencil* target, float depth, u8 stencil) = 0;
//     virtual void DispatchCompute(ui32 x, ui32 y, ui32 z) = 0;
//     virtual void CopyToBuffer(IBuffer* buffer, void* data, u64 size) = 0;
//     virtual void Draw(PrimitiveType type, u64 vertexCount, u64 vertexOffset = 0) = 0;
//     virtual void DrawIndexed(PrimitiveType type, IBuffer* indexBuffer, u64 indexCount, u64 indexOffset = 0) = 0;

//     virtual IDeviceContext* getContext() = 0;
//     virtual IDevice* getDevice() = 0;

//     virtual void Shutdown() = 0;
// };


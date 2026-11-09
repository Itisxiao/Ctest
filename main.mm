#import <Cocoa/Cocoa.h>
#import <ScreenCaptureKit/ScreenCaptureKit.h>
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#import <QuartzCore/CAMetalLayer.h>
#import <CoreVideo/CoreVideo.h>
#import <CoreMedia/CoreMedia.h>
#include <iostream>

@interface MetalView : NSView
@property (nonatomic, strong) CAMetalLayer *metalLayer;
@property (nonatomic, strong) id<MTLCommandQueue> commandQueue;
@property (nonatomic, strong) id<MTLRenderPipelineState> pipeline;
@property (nonatomic, strong) id<MTLSamplerState> sampler;
@property (nonatomic) CVMetalTextureCacheRef textureCache;
- (void)displayPixelBuffer:(CVPixelBufferRef)pixelBuffer;
@end

@implementation MetalView
+ (Class)layerClass { return [CAMetalLayer class]; }
- (instancetype)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        self.wantsLayer = YES;
        _metalLayer = [CAMetalLayer layer];
        _metalLayer.device = MTLCreateSystemDefaultDevice();
        _commandQueue = [_metalLayer.device newCommandQueue];
        _metalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
        _metalLayer.framebufferOnly = YES;
        CVMetalTextureCacheCreate(kCFAllocatorDefault, NULL, _metalLayer.device, NULL, &_textureCache);
        NSError *error = nil;
        NSString *source = @"#include <metal_stdlib>\nusing namespace metal;\nstruct V{float4 p [[position]]; float2 uv;};\nvertex V vmain(uint id [[vertex_id]]){float2 p[4]={{-1,-1},{1,-1},{-1,1},{1,1}};float2 u[4]={{0,1},{1,1},{0,0},{1,0}};V o;o.p=float4(p[id],0,1);o.uv=u[id];return o;}\nfragment float4 fmain(V in [[stage_in]], texture2d<float> t [[texture(0)]], sampler s [[sampler(0)]]){return t.sample(s,in.uv);}";
        id<MTLLibrary> library = [_metalLayer.device newLibraryWithSource:source options:nil error:&error];
        if (library) {
            MTLRenderPipelineDescriptor *descriptor = [[MTLRenderPipelineDescriptor alloc] init];
            descriptor.vertexFunction = [library newFunctionWithName:@"vmain"];
            descriptor.fragmentFunction = [library newFunctionWithName:@"fmain"];
            descriptor.colorAttachments[0].pixelFormat = _metalLayer.pixelFormat;
            _pipeline = [_metalLayer.device newRenderPipelineStateWithDescriptor:descriptor error:&error];
            MTLSamplerDescriptor *samplerDescriptor = [[MTLSamplerDescriptor alloc] init];
            samplerDescriptor.minFilter = MTLSamplerMinMagFilterLinear;
            samplerDescriptor.magFilter = MTLSamplerMinMagFilterLinear;
            _sampler = [_metalLayer.device newSamplerStateWithDescriptor:samplerDescriptor];
        }
        if (error) NSLog(@"Metal pipeline error: %@", error);
        _metalLayer.contentsScale = NSScreen.mainScreen.backingScaleFactor;
        self.layer = _metalLayer;
    }
    return self;
}
- (BOOL)isFlipped { return YES; }
- (void)layout {
    [super layout];
    self.metalLayer.frame = self.bounds;
    self.metalLayer.drawableSize = CGSizeMake(self.bounds.size.width * self.window.backingScaleFactor,
                                               self.bounds.size.height * self.window.backingScaleFactor);
}
- (void)displayPixelBuffer:(CVPixelBufferRef)pixelBuffer {
    if (!pixelBuffer || !self.pipeline || !self.textureCache) return;
    OSType pixelFormat = CVPixelBufferGetPixelFormatType(pixelBuffer);
    if (pixelFormat != kCVPixelFormatType_32BGRA) {
        static BOOL logged = NO;
        if (!logged) { NSLog(@"Unsupported ScreenCaptureKit pixel format: %u", (unsigned)pixelFormat); logged = YES; }
        return;
    }
    size_t width = CVPixelBufferGetWidth(pixelBuffer), height = CVPixelBufferGetHeight(pixelBuffer);
    CVMetalTextureRef cvTexture = NULL;
    CVReturn status = CVMetalTextureCacheCreateTextureFromImage(kCFAllocatorDefault, self.textureCache, pixelBuffer, NULL,
                                                                 MTLPixelFormatBGRA8Unorm, width, height, 0, &cvTexture);
    if (status != kCVReturnSuccess || !cvTexture) return;
    id<MTLTexture> texture = CVMetalTextureGetTexture(cvTexture);
    id<CAMetalDrawable> drawable = [self.metalLayer nextDrawable];
    if (texture && drawable) {
        MTLRenderPassDescriptor *pass = [MTLRenderPassDescriptor renderPassDescriptor];
        pass.colorAttachments[0].texture = drawable.texture;
        pass.colorAttachments[0].loadAction = MTLLoadActionClear;
        pass.colorAttachments[0].storeAction = MTLStoreActionStore;
        id<MTLCommandBuffer> command = [self.commandQueue commandBuffer];
        id<MTLRenderCommandEncoder> encoder = [command renderCommandEncoderWithDescriptor:pass];
        [encoder setRenderPipelineState:self.pipeline];
        [encoder setFragmentTexture:texture atIndex:0];
        [encoder setFragmentSamplerState:self.sampler atIndex:0];
        [encoder drawPrimitives:MTLPrimitiveTypeTriangleStrip vertexStart:0 vertexCount:4];
        [encoder endEncoding];
        [command presentDrawable:drawable];
        [command commit];
    }
    CFRelease(cvTexture);
}
@end

@interface StreamHandler : NSObject <SCStreamDelegate, SCStreamOutput>
@property (nonatomic, weak) MetalView *metalView;
@property (atomic) uint64_t latestFrameID;
@end
@implementation StreamHandler
- (void)stream:(SCStream *)stream didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer ofType:(SCStreamOutputType)type {
    if (type != SCStreamOutputTypeScreen) return;
    CVPixelBufferRef pixelBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    if (!pixelBuffer) return;
    CVPixelBufferRetain(pixelBuffer);
    uint64_t frameID = __sync_add_and_fetch(&_latestFrameID, 1);
    dispatch_async(dispatch_get_main_queue(), ^{
        if (frameID == self.latestFrameID) [self.metalView displayPixelBuffer:pixelBuffer];
        CVPixelBufferRelease(pixelBuffer);
    });
}
- (void)stream:(SCStream *)stream didStopWithError:(NSError *)error {
    std::cerr << "Stream stopped with error: " << [[error localizedDescription] UTF8String] << std::endl;
}
@end

@interface AppDelegate : NSObject <NSApplicationDelegate>
@property (nonatomic, strong) NSWindow *window;
@property (nonatomic, strong) MetalView *metalView;
@property (nonatomic, strong) StreamHandler *handler;
@property (nonatomic, strong) SCStream *stream;
@end
@implementation AppDelegate
- (void)applicationDidFinishLaunching:(NSNotification *)notification {
    NSRect frame = NSMakeRect(100, 100, 1280, 720);
    self.window = [[NSWindow alloc] initWithContentRect:frame styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable backing:NSBackingStoreBuffered defer:NO];
    [self.window setTitle:@"macOS 15 ScreenCaptureKit Demo"];
    self.metalView = [[MetalView alloc] initWithFrame:NSMakeRect(0, 0, frame.size.width, frame.size.height)];
    [self.window setContentView:self.metalView];
    self.handler = [[StreamHandler alloc] init];
    self.handler.metalView = self.metalView;
    [self.window makeKeyAndOrderFront:nil];
    [NSApp activateIgnoringOtherApps:YES];
    [self startScreenCapture];
}
- (void)startScreenCapture {
    std::cout << "Requesting shareable content..." << std::endl;
    [SCShareableContent getShareableContentWithCompletionHandler:^(SCShareableContent *content, NSError *error) {
        if (error || !content) { std::cerr << "Failed to get content. Did you grant Screen Recording permission?" << std::endl; if (error) std::cerr << [[error localizedDescription] UTF8String] << std::endl; return; }
        if (content.displays.count == 0) { std::cerr << "No displays found." << std::endl; return; }
        SCDisplay *display = content.displays[0];
        SCContentFilter *filter = [[SCContentFilter alloc] initWithDisplay:display excludingWindows:@[]];
        SCStreamConfiguration *config = [[SCStreamConfiguration alloc] init];
        config.width = 1920; config.height = 1080; config.pixelFormat = kCVPixelFormatType_32BGRA; config.minimumFrameInterval = CMTimeMake(1, 60); config.queueDepth = 5; config.scalesToFit = YES;
        self.stream = [[SCStream alloc] initWithFilter:filter configuration:config delegate:self.handler];
        dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0);
        [self.stream addStreamOutput:self.handler type:SCStreamOutputTypeScreen sampleHandlerQueue:queue error:nil];
        [self.stream startCaptureWithCompletionHandler:^(NSError *startError) {
            if (startError) std::cerr << "Failed to start capture: " << [[startError localizedDescription] UTF8String] << std::endl;
            else std::cout << "Capture started successfully! (Press Cmd+Q to exit)" << std::endl;
        }];
    }];
}
- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender { return YES; }
- (void)applicationWillTerminate:(NSNotification *)notification { if (self.stream) [self.stream stopCaptureWithCompletionHandler:nil]; }
@end
int main(int argc, const char * argv[]) {
    @autoreleasepool { NSApplication *app = [NSApplication sharedApplication]; AppDelegate *delegate = [[AppDelegate alloc] init]; [app setDelegate:delegate]; [app setActivationPolicy:NSApplicationActivationPolicyRegular]; [app run]; }
    return 0;
}

/*
ScreenCaptureKit 是"采集服务"，它跨进程从 WindowServer 拿帧，交付格式必须是系统定义的 CMSampleBuffer——这是 CoreMedia 的通用容器，不是 Metal 的。你不能让 SCK 直接吐 MTLTexture，因为采集框架不该依赖渲染框架。

CVPixelBuffer 是"图像容器"，它包着 IOSurface（真正的共享内存）。CoreVideo 用它来统一描述"一张图"，但它不是 GPU 资源类型。你要用 GPU 就得把它映射成 MTLTexture——注意是映射不是拷贝。

MTLTexture 是"GPU 资源句柄"，Metal 只认这个。所以 CVMetalTextureCache 的角色就是"把 IOSurface 包成 MTLTexture 的翻译官"，同一个 IOSurface 两边都能看。

CAMetalLayer.nextDrawable 是"上屏契约"，屏幕上的内容由 WindowServer 合成，你必须把结果写进它给你的 drawable（又是一块 IOSurface），不能自己随便找块显存就往屏幕上怼。

Render Pipeline 是"唯一必须做的活"：采集格式（常见 BGRA 或 YCbCr 10bit）≠ 显示格式，分辨率也可能不同，还可能要做色彩空间/HDR 转换。这一步必须跑 shader，绕不开。 
*/
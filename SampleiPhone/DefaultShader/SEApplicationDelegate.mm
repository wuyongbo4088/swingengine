#import "SEApplicationDelegate.h"
#include "SEWindowApplication.h"
#include "SEiPhoneOES2Renderer.h"

using namespace Swing;

static const float gs_fFPS = 60.0f;

@implementation ApplicationDelegate
//----------------------------------------------------------------------------
- (void) update
{
    WindowApplication* pTheApp = 
        (WindowApplication*)Application::TheApplication;
    SE_ASSERT( pTheApp );

    // Ӧ�ó���ʵʱ�߼�/��Ⱦ��ں���.
    pTheApp->OnIdle();
}
//----------------------------------------------------------------------------
- (void) applicationDidFinishLaunching:(UIApplication*)application
{
    // ȷ��applicationʵ���Ѿ�����.
    WindowApplication* pTheApp = 
        (WindowApplication*)Application::TheApplication;
    SE_ASSERT( pTheApp );	

    // ��ʵ��.
    // Ŀǰֻ֧��ȫ��Ļ����.
    // ���������С�Ĵ���.	
    CGRect rect = [[UIScreen mainScreen] bounds];

    // ��������.
    AppWindow = [[UIWindow alloc] initWithFrame:rect];

    pTheApp->SetWindowID((int)AppWindow);

    // ������Ⱦ��.
    pTheApp->SetRenderer(SE_NEW iPhoneOES2Renderer(AppWindow, 
        pTheApp->GetFormat(), pTheApp->GetDepth(), pTheApp->GetStencil(), 
        pTheApp->GetBuffering(), pTheApp->GetMultisampling(), 0, 0, 
        rect.size.width, rect.size.height));

    // Ӧ�ó����ʼ����ں���,���ڷ���Ӧ�ó������������ڵ������Դ.
    // ����������Swing::Object���������ʵ��Ӧ�������ʼ��.	
    pTheApp->OnInitialize();

    // ��ʾ����.
    [AppWindow makeKeyAndVisible];

    // ����������Ⱦ��timer,��ʱ�ص��������update����.
    [NSTimer scheduledTimerWithTimeInterval:(1.0 / gs_fFPS) target:self 
        selector:@selector(update) userInfo:nil repeats:YES];
}
//----------------------------------------------------------------------------
- (void) dealloc
{
    WindowApplication* pTheApp = 
        (WindowApplication*)Application::TheApplication;
    SE_ASSERT( pTheApp );

    // Ӧ�ó������������ں���,�����ͷ�Ӧ�ó������������ڵ������Դ.
    // ����������Swing::Object���������ʵ��Ӧ�������ͷ�.
    // ���Ӧ�ó����ͷ�renderer.
    pTheApp->OnTerminate();	

    // �ͷ�������.
    [AppWindow release];

    [super dealloc];
}
//----------------------------------------------------------------------------
@end




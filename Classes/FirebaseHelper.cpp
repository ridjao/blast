#include "FirebaseHelper.h"

#if defined(__ANDROID__)
#include <android/log.h>
#include <jni.h>
#include "cocos2d.h"
#include "firebase/admob.h"
#include "firebase/admob/banner_view.h"
#include "firebase/app.h"
#include "firebase/future.h"
#include "platform/android/jni/JniHelper.h"

static firebase::admob::BannerView* bannerView;
#endif

firebase::admob::AdParent getAdParent() {
#if defined(__ANDROID__)
	return cocos2d::JniHelper::getActivity();
#else
	return nullptr;
#endif
}

void FirebaseHelper::initializeFirebase() {
#if defined(__ANDROID__)
	firebase::App* app = firebase::App::Create(firebase::AppOptions(), cocos2d::JniHelper::getEnv(), cocos2d::JniHelper::getActivity());
	firebase::admob::Initialize(*app, "ca-app-pub-4351684299804113~8429130142");
#endif
}

void FirebaseHelper::createBannerAd() {
#if defined(__ANDROID__)
    static bool initialized = false;
    if (!initialized) {
        const char* kBannerAdUnit = "ca-app-pub-4351684299804113/2909172161"; //<-- REAL
        //const char* kBannerAdUnit = "ca-app-pub-3940256099942544/6300978111"; //<-- TEST
        bannerView = new firebase::admob::BannerView();
        firebase::admob::AdSize adSize;
        adSize.ad_size_type = firebase::admob::kAdSizeStandard;
        adSize.width = 320;
        CCLOG("height: %f", cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize().height);
        if (cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize().height <= 1280.0f)
            adSize.height = 50;
        else
            adSize.height = 100;
        bannerView->Initialize(getAdParent(), kBannerAdUnit, adSize);
        initialized = true;
    }
#endif
}

void FirebaseHelper::showBannerAd() {
#if defined(__ANDROID__)
    if (bannerView->InitializeLastResult().status() == firebase::kFutureStatusComplete) {
        static bool moved = false;
        if (!moved) {
            bannerView->MoveTo(firebase::admob::BannerView::kPositionBottom);
            bannerView->Show();
            moved = true;
        }

        static int calls = 0;
        bool refresh = false; // ++calls % 60 == 0;
        if (refresh || bannerView->LoadAdLastResult().status() == firebase::kFutureStatusInvalid) {
            firebase::admob::AdRequest request = {};
            bannerView->LoadAd(request);
        }
    }
#endif
}

#ifndef __FIREBASE_HELPER_H__
#define __FIREBASE_HELPER_H__

#include "firebase/admob/types.h"

class FirebaseHelper
{
public:
	static void initializeFirebase();
	static void createBannerAd();
	static void showBannerAd();
};


#endif // __FIREBASE_HELPER_H__

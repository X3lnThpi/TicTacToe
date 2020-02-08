FROM fsk00x/cocos2dx-docker:v1
VOLUME https://console.cloud.google.com/storage/browser/cocos2dx_artifacts
COPY Classes* /Classes

COPY Resources* /Resources
COPY bin/debug/android* /bin/debug/android
COPY cocos2d* /cocos2d
COPY firebase_cpp_sdk* /firebase_cpp_sdk
COPY proj.android* /proj.android
CMD cd proj.android
CMD cocos compile -p android
CMD pwd

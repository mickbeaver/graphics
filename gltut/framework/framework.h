#ifndef GLTUT_FRAMEWORK_H
#define GLTUT_FRAMEWORK_H

typedef struct GltutDefaultSettings {
    int windowWidth;
    int windowHeight;
    const char *windowTitle;
} GltutDefaultSettings;

#define GLTUT_DEFAULT_SETTINGS_INITIALIZER \
    {.windowWidth=640, .windowHeight=480, .windowTitle="gltut"}

void    gltutPostRenderSystemInit(void);
void    gltutDisplay(void);
void    gltutReshape(int width, int height);
void    gltutDefaultSettingsInit(GltutDefaultSettings *settings);

#endif // GLTUT_FRAMEWORK_H

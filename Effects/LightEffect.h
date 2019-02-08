#ifndef _LIGHTEFFECT_H_
#define _LIGHTEFFECT_H_

#include "Effect.h"


class LightEffect : public Effect
{
public:
    static LightEffect* create();
    
    void setLightPos(const cocos2d::Vec3& pos, int id);
	int addLight(const cocos2d::Vec3 &pos);
	void moveLightTo(const cocos2d::Vec2 &pos, int id);
	void rmvLight(int id);
    void setLightColor(const cocos2d::Color3B& color);
    void setAmbientLightColor(const cocos2d::Color3B& color);
    void setBrightness(float value);
    void setLightCutoffRadius(float value);
    void setLightHalfRadius(float value);
    
    void prepareForRender(cocos2d::Sprite *sprite, cocos2d::Texture2D *normalmap);
	//void prepareForRender(cocos2d::LayerColor *layer);
    
    cocos2d::Color3B getLightColor() const { return _lightColor; }
    cocos2d::Color3B getAmbientLightColor() const { return _ambientLightColor; }

protected:
    bool init();

	static const int MAX_LIGHTS = 10;

    cocos2d::Vec3 _lightPos[MAX_LIGHTS];
    cocos2d::Color3B _lightColor;
    cocos2d::Color3B _ambientLightColor;
    float _lightCutoffRadius;
    float _lightHalfRadius;
    float _brightness;
	int _num_lights = 0;
	cocos2d::Texture2D * layer_normalmap;
};

#endif

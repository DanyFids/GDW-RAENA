/****************************************************************************
 Copyright (c) 2014 Chukong Technologies Inc.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

/*
 * based on cocos2d-x-3.6/tests/cpp-tests/Classes/ShaderTest/ShaderTest2.cpp
 * extended by CodeAndWeb GmbH https://www.codeandweb.com
 */

#include "LightEffect.h"
#include "EffectSprite.h"

USING_NS_CC;

LightEffect* LightEffect::create()
{
    LightEffect *normalMappedSprite = new (std::nothrow) LightEffect();
    if (normalMappedSprite && normalMappedSprite->init())
    {
        normalMappedSprite->autorelease();
        return normalMappedSprite;
    }
    CC_SAFE_DELETE(normalMappedSprite);
    return nullptr;
}

bool LightEffect::init()
{
    if (initGLProgramState("spot_light.frag"))
    {
        setLightColor(cocos2d::Color3B::WHITE);
        setAmbientLightColor(cocos2d::Color3B(127,127,127));
        setLightCutoffRadius(500.0f);
        setLightHalfRadius(0.5f);

		layer_normalmap = cocos2d::Director::getInstance()->getTextureCache()->addImage("layerNorm.png");

        return true;
    }
    return false;
}

void LightEffect::setLightPos(const Vec3 &pos, int id)
{
	if (id < _num_lights) {
		_lightPos[id] = pos;
	}
	else {
		printf("ERROR: Light ID out of range.");
	}
}

int LightEffect::addLight(const Vec3 &pos) {
	if (_num_lights < MAX_LIGHTS) {
		_lightPos[_num_lights] = pos;
		_num_lights++;
		return _num_lights - 1;
	}
	else {
		printf("ERROR: Max amount of lights already reached!");
		return -1;
	}
}

void LightEffect::moveLightTo(const cocos2d::Vec2 & pos, int id)
{
	if (id < 0 || id > _num_lights) {
		printf("ERROR: ID out of range!");
	}
	else {
		_lightPos[id].x = pos.x;
		_lightPos[id].y = pos.y;
	}
}

void LightEffect::rmvLight(int id)
{
	if (id < 0 || id > _num_lights) {
		printf("ERROR: ID out of range!");
	}
	else {
		_num_lights--;
		for (int i = id; i < _num_lights; i++) {
			_lightPos[i] = _lightPos[i + 1];
		}
	}
}

void LightEffect::setLightColor(const Color3B &color)
{
    _lightColor = color;
    getGLProgramState()->setUniformVec3("u_lightColor", Vec3(_lightColor.r,_lightColor.g,_lightColor.b)/255.0f);
}

void LightEffect::setAmbientLightColor(const Color3B &color)
{
    _ambientLightColor = color;
    getGLProgramState()->setUniformVec3("u_ambientColor", Vec3(_ambientLightColor.r,_ambientLightColor.g,_ambientLightColor.b)/255.0f);
}

void LightEffect::setBrightness(float value)
{
    _brightness = value;
    getGLProgramState()->setUniformFloat("u_brightness", value);
}

void LightEffect::setLightCutoffRadius(float value)
{
    _lightCutoffRadius = std::max(1.0f, value);
    getGLProgramState()->setUniformFloat("u_cutoffRadius", _lightCutoffRadius);
}

void LightEffect::setLightHalfRadius(float value)
{
    _lightHalfRadius = std::max(0.01f, std::min(0.99f, value));
    getGLProgramState()->setUniformFloat("u_halfRadius", _lightHalfRadius);
}

void LightEffect::prepareForRender(Sprite *sprite, Texture2D *normalmap)
{
    auto gl = getGLProgramState();

    gl->setUniformVec2("u_contentSize", sprite->getContentSize());

	for(int l = 0; l < MAX_LIGHTS; l++)
	{
		Point posRelToSprite = PointApplyAffineTransform(Point(_lightPos[l].x, _lightPos[l].y), sprite->getWorldToNodeAffineTransform());
		gl->setUniformVec3("u_lightPos_" + std::to_string(l + 1), cocos2d::Vec3(posRelToSprite.x, posRelToSprite.y, _lightPos[l].z));
	}

	//posRelToSprite = PointApplyAffineTransform(Point(_lightPos[1].x, _lightPos[1].y), sprite->getWorldToNodeAffineTransform());
	//gl->setUniformVec3("u_lightPos_2", cocos2d::Vec3(posRelToSprite.x, posRelToSprite.y, _lightPos[1].z));

	gl->setUniformInt("u_numLights", _num_lights);

    gl->setUniformTexture("u_normals", normalmap);

    SpriteFrame *frame = sprite->getSpriteFrame();
    Size untrimmedSize = frame->getOriginalSize();
    Size trimmedSize = frame->getRect().size;
    Vec2 framePos = frame->getRect().origin;
    Size texSize = frame->getTexture()->getContentSize();
    
    // set sprite position in sheet
    gl->setUniformVec2("u_spritePosInSheet", Vec2(framePos.x / texSize.width, framePos.y / texSize.height));
    gl->setUniformVec2("u_spriteSizeRelToSheet", Vec2(untrimmedSize.width / texSize.width, untrimmedSize.height / texSize.height));
    gl->setUniformInt("u_spriteRotated", frame->isRotated());
	
    
    // set offset of trimmed sprite
    Vec2 bottomLeft = frame->getOffset() + (untrimmedSize - trimmedSize) / 2;
    Vec2 cornerOffset = frame->isRotated() ? Vec2(bottomLeft.y, bottomLeft.x)
                                           : Vec2(bottomLeft.x, untrimmedSize.height - trimmedSize.height - bottomLeft.y);
    gl->setUniformVec2("u_spriteOffset", cornerOffset);

}

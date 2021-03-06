//
//  NuoModelMaterialed.hpp
//  ModelViewer
//
//  Created by dfeng on 9/7/16.
//  Copyright © 2016 middleware. All rights reserved.
//

#ifndef NuoModelMaterialedBasic_hpp
#define NuoModelMaterialedBasic_hpp


#include "tiny_obj_loader.h"
#include "NuoModelBase.h"
#include "NuoModelTextured.h"
#include "NuoMaterial.h"




template <class ItemBase>
class NuoModelMaterialedBasicBase : virtual public NuoModelCommon<ItemBase>
{

private:
    bool _hasTransparent { false };
    
public:
    
    virtual void AddMaterial(const NuoMaterial& material) override;
    virtual bool HasTransparent() override;
    
};



struct NuoItemMaterialedTexturedBasic
{
    vector_float4 _position;
    vector_float4 _normal;
    vector_float2 _texCoord;
    
    vector_float3 _diffuse;
    vector_float3 _ambient;
    vector_float3 _specular;
    vector_float2 _shinessDisolve;
    
    NuoItemMaterialedTexturedBasic();
    
    bool operator == (const NuoItemMaterialedTexturedBasic& other);
};



class NuoModelMaterialedTextured : virtual public NuoModelTextureBase<NuoItemMaterialedTexturedBasic>,
                                           public NuoModelMaterialedBasicBase<NuoItemMaterialedTexturedBasic>
{
public:
    NuoModelMaterialedTextured();
    
    virtual void GenerateTangents() override;
    virtual void SetTexturePathBump(const std::string texPath) override;
    virtual std::string GetTexturePathBump() override;
};




struct NuoItermMaterialedBumpedTextured
{
    vector_float4 _position;
    vector_float4 _normal;
    vector_float4 _tangent;
    vector_float2 _texCoord;
    
    vector_float3 _diffuse;
    vector_float3 _ambient;
    vector_float3 _specular;
    vector_float2 _shinessDisolve;
    
    NuoItermMaterialedBumpedTextured();
    
    bool operator == (const NuoItermMaterialedBumpedTextured& other);
};




class NuoModelMaterialedBumpedTextured : virtual public NuoModelTexturedWithTangentBase<NuoItermMaterialedBumpedTextured>,
                                         virtual public NuoModelMaterialedBasicBase<NuoItermMaterialedBumpedTextured>
{
};




struct NuoItemMaterialedBasic
{
    vector_float4 _position;
    vector_float4 _normal;
    
    vector_float3 _diffuse;
    vector_float3 _ambient;
    vector_float3 _specular;
    vector_float2 _shinessDisolve;
    
    NuoItemMaterialedBasic();
    
    bool operator == (const NuoItemMaterialedBasic& other);
};



class NuoModelMaterialed : public NuoModelMaterialedBasicBase<NuoItemMaterialedBasic>
{

public:
    virtual void AddTexCoord(size_t sourceIndex, const std::vector<float>& texCoordBuffer) override;
    virtual void GenerateTangents() override;
    
    virtual void SetTexturePathDiffuse(const std::string texPath) override;
    virtual std::string GetTexturePathDiffuse() override;
    virtual void SetTexturePathOpacity(const std::string texPath) override;
    virtual std::string GetTexturePathOpacity() override;
    virtual void SetTexturePathBump(const std::string texPath) override;
    virtual std::string GetTexturePathBump() override;
};



template <class ItemBase>
void NuoModelMaterialedBasicBase<ItemBase>::AddMaterial(const NuoMaterial& material)
{
    size_t targetOffset = NuoModelCommon<ItemBase>::_buffer.size() - 1;
    
    NuoModelCommon<ItemBase>::_buffer[targetOffset]._diffuse.x = material.diffuse[0];
    NuoModelCommon<ItemBase>::_buffer[targetOffset]._diffuse.y = material.diffuse[1];
    NuoModelCommon<ItemBase>::_buffer[targetOffset]._diffuse.z = material.diffuse[2];
    
    NuoModelCommon<ItemBase>::_buffer[targetOffset]._ambient.x = material.ambient[0];
    NuoModelCommon<ItemBase>::_buffer[targetOffset]._ambient.y = material.ambient[1];
    NuoModelCommon<ItemBase>::_buffer[targetOffset]._ambient.z = material.ambient[2];
    
    NuoModelCommon<ItemBase>::_buffer[targetOffset]._specular.x = material.specular[0];
    NuoModelCommon<ItemBase>::_buffer[targetOffset]._specular.y = material.specular[1];
    NuoModelCommon<ItemBase>::_buffer[targetOffset]._specular.z = material.specular[2];
    
    NuoModelCommon<ItemBase>::_buffer[targetOffset]._shinessDisolve.x = material.shininess;
    NuoModelCommon<ItemBase>::_buffer[targetOffset]._shinessDisolve.y = material.dissolve;
    
    if (material.dissolve - 1.0 < -1e-3)
        _hasTransparent = true;
}


template <class ItemBase>
bool NuoModelMaterialedBasicBase<ItemBase>::HasTransparent()
{
    return _hasTransparent;
}



#endif /* NuoModelMaterialed_hpp */

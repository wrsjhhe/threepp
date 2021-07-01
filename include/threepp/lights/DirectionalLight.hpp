// https://github.com/mrdoob/three.js/blob/r129/src/lights/DirectionalLight.js

#ifndef THREEPP_DIRECTIONALLIGHT_HPP
#define THREEPP_DIRECTIONALLIGHT_HPP

#include "threepp/lights/Light.hpp"
#include "threepp/lights/DirectionalLightShadow.hpp"

namespace threepp {

    class DirectionalLight : public Light {

    public:

        Vector3 position;
        Object3D* target = nullptr;

        DirectionalLightShadow shadow;

        DirectionalLight(const DirectionalLight &) = delete;

        template<class T>
        static std::shared_ptr<DirectionalLight> create(T color, std::optional<float> intensity = std::nullopt) {
            return std::shared_ptr<DirectionalLight>(new DirectionalLight(color, intensity));
        }

        void dispose() override {

            this->shadow.dispose();
        }
        
    protected:
        template<class T>
        DirectionalLight(T color, std::optional<float> intensity) : Light(color, intensity) {

            this->position.copy(Object3D::defaultUp);
            this->updateMatrix();
        }

    };

}// namespace threepp

#endif//THREEPP_DIRECTIONALLIGHT_HPP

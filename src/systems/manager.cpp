

#include <SFML/Graphics.hpp>
#include "systems/manager.hpp"

const sf::Texture& TextureManager::load(TextureID id, const std::string& filename){
    auto& texture = textures[id];

    if (!texture.loadFromFile(filename)) {
        exit(1);

    }

    return texture;
}

const sf::Texture& TextureManager::get(TextureID id) {
    return textures.at(id); // Like [id]
}





#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <SFML/Graphics.hpp>


// class so it doesn't clash, use TextureID::Missile instead
enum class TextureID
{
    Missile
};

class TextureManager {
public:
    // const to avoid copying textures &
    const sf::Texture& load(TextureID id, const std::string& filename);
    const sf::Texture& get(TextureID id);

private:
    std::unordered_map<TextureID, sf::Texture> textures;
};


#endif // MANAGER_HPP


#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include "sprite.h"
#include <SFML/Graphics.hpp>

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>


using SpriteID = size_t;

class SpriteManager {

    public:

        sf::Texture& registerTexture(const std::string& filepath) {

            // check if a filepath is already registered
            auto it = mTextures.find(filepath);

            // if filepath exists, return a reference to it by dereferencing the pointer to the texture 
            // in the key value pair returned to us by find() and accessed via second
            // it->first == key(filepath) it->second == &texture(value)
            if (it != mTextures.end()) {
                return *it->second;
            }

            // if filepath doesnt exist, create a new texture held by a unique_ptr
            // and return a reference to it
            auto texture = std::make_unique<sf::Texture>(filepath);
            auto& result = *texture;
            mTextures.emplace(filepath, std::move(texture));

            return result;
        
        }    

        // allows any number of argument types
        template <typename... Args>
        SpriteID createSprite(Args&&... args) {
            // preserves the arguments' value categories when passing them to the sprite constructor
            mSprites.push_back(std::make_unique<Sprite>(std::forward<Args>(args)...));
            
            // returns the index of the sprite we just added
            return mSprites.size() - 1;
        }

        Sprite& getSprite(SpriteID id) {
            // at(id) instead of *mSprites[id] since at() checks and throws exception if out of index
            return *mSprites.at(id);
        }

    private:
        
        std::unordered_map<std::string, std::unique_ptr<sf::Texture>>mTextures;
        std::vector<std::unique_ptr<Sprite>> mSprites;

};

#endif  // SPRITE_MANAGER_H

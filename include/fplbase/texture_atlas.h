// Copyright 2015 Google Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef FPLBASE_TEXTURE_ATLAS_H
#define FPLBASE_TEXTURE_ATLAS_H

#include <map>
#include <string>
#include <vector>

#include "fplbase/config.h"  // Must come first.

namespace fplbase {

using mathfu::vec2i;
using mathfu::vec4i;

class Texture;

/// @class TextureAtlas
/// @brief Texture coordinate dictionary.
///
/// A TextureAtlas supports sprite sheets by containing one texture and a list
/// of subtexture bounding boxes, which can also be indexed by name using the
/// index_map.
///
/// @warning This is will very likely be refactored.
class TextureAtlas {
 public:
  TextureAtlas() : atlas_texture_(nullptr) {}
  ~TextureAtlas() { Delete(); }

  /// @brief Delete the texture associated with this atlas.
  void Delete() {
    atlas_texture_->Delete();
    atlas_texture_ = nullptr;
  }

  /// @brief Get the bounds of a subtexture associated with name.
  ///
  /// @param name Name of the subtexture to lookup.
  /// @returns Bounds of the subtexture or nullptr if the specified name isn't
  /// found.
  const vec4i *GetBounds(const std::string &name) {
    auto index_iter = index_map_.find(name);
    if (index_iter != index_map_.end()) {
      return &subtexture_bounds_[index_iter->second];
    }
    return nullptr;
  }

  /// @brief Get the texture associated with this atlas.
  /// @return Pointer to the texture associated with this atlas.
  const Texture *atlas_texture() const { return atlas_texture_; }
  /// @brief Get the texture associated with this atlas.
  /// @return Pointer to the texture associated with this atlas.
  Texture *atlas_texture() { return atlas_texture_; }
  /// @brief Set the texture associated with this atlas.
  void set_atlas_texture(Texture *atlas_texture) {
    atlas_texture_ = atlas_texture;
    if (atlas_texture && atlas_texture->id()) {
      set_size(atlas_texture->size());
    }
  }

  /// @brief Get the size of the overall texture.
  /// @return Size of the overall texture.
  const vec2i &size() const { return size_; }
  /// @brief Set the size of the overall texture.
  void set_size(const vec2i &size) { size_ = size; }

  /// @brief Get a vector of the bounds of each subtexture in this atlas.
  ///
  /// Each element of the vector consists of (offsetx, offsety, sizex, sizey)
  /// where offsetx / offsety are coordinates relative to the underlying
  /// atlased texture and sizex / sizey are the dimensions of the subtexture.
  ///
  /// @returns Vector of subtexture bounds.
  const std::vector<vec4i> &subtexture_bounds() const {
    return subtexture_bounds_;
  }
  std::vector<vec4i> &subtexture_bounds() { return subtexture_bounds_; }

  /// @brief Get a map of subtexture names to subtexture offsets.
  ///
  /// Each entry in the map can be used to lookup the subtexture bounds in
  /// the vector returned by @ref subtexture_bounds().
  ///
  /// @return A map of subtexture names to indices in vector returned by
  /// @ref subtexture_bounds().
  const std::map<std::string, size_t> &index_map() const { return index_map_; }
  std::map<std::string, size_t> &index_map() { return index_map_; }

 private:
  // Texture being used by this atlas.
  Texture *atlas_texture_;
  // Size of the texture/
  vec2i size_;
  // List of bounds (offsetx, offsety, sizex, sizey) of each subtexture.
  std::vector<vec4i> subtexture_bounds_;
  // Map of subtexture names to indices into subtexture_bounds_.
  std::map<std::string, size_t> index_map_;
};

}  // namespace fplbase

#endif  // FPLBASE_TEXTURE_ATLAS_H

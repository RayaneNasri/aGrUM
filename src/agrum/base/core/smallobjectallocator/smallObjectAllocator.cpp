/**
 *
 *   Copyright (c) 2005-2024  by Pierre-Henri WUILLEMIN(_at_LIP6) & Christophe GONZALES(_at_AMU)
 *   info_at_agrum_dot_org
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


/**
 * @file
 * @brief Sources of gum::SmallObjectAllocator
 *
 * @author Pierre-Henri WUILLEMIN(_at_LIP6) and Jean-Christophe MAGNAN and Christophe
 * GONZALES(_at_AMU)
 *
 */
// ============================================================================
#include <agrum/base/core/smallobjectallocator/fixedAllocator.h>
#include <agrum/base/core/smallobjectallocator/smallObjectAllocator.h>

// ============================================================================
namespace gum {


  /*
   * @variable The default size of chunck of memory.
   * These chuncks are pre-allocated memory space which are
   * then split in small memory space of the size of a small object
   */
  const size_t SmallObjectAllocator::GUM_DEFAULT_CHUNK_SIZE = 8096;

  /*
   * @variable The default maximal size under which an object is considered
   * small. If an object size is over this limit, the normal new allocator is
   * called.
   */
  const size_t SmallObjectAllocator::GUM_DEFAULT_MAX_OBJECT_SIZE = 512;


}   // namespace gum

#ifdef GUM_NO_INLINE
#  include <agrum/base/core/smallobjectallocator/smallObjectAllocator_inl.h>
#endif

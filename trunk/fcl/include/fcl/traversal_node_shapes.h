/*
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2011, Willow Garage, Inc.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Willow Garage, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/** \author Jia Pan */


#ifndef FCL_TRAVERSAL_NODE_SHAPES_H
#define FCL_TRAVERSAL_NODE_SHAPES_H

#include "fcl/collision_data.h"
#include "fcl/traversal_node_base.h"
#include "fcl/narrowphase/narrowphase.h"

namespace fcl
{

template<typename S1, typename S2, typename NarrowPhaseSolver>
class ShapeCollisionTraversalNode : public CollisionTraversalNodeBase
{
public:
  ShapeCollisionTraversalNode() : CollisionTraversalNodeBase()
  {
    model1 = NULL;
    model2 = NULL;

    is_collision = false;

    nsolver = NULL;
  }

  bool BVTesting(int, int) const
  {
    return false;
  }

  void leafTesting(int, int) const
  {
    if(request.enable_contact)
      is_collision = nsolver->shapeIntersect(*model1, tf1, *model2, tf2, &contact_point, &penetration_depth, &normal);
    else
      is_collision = nsolver->shapeIntersect(*model1, tf1, *model2, tf2, NULL, NULL, NULL);
  }

  const S1* model1;

  const S2* model2;

  mutable Vec3f normal;

  mutable Vec3f contact_point;

  mutable FCL_REAL penetration_depth;

  CollisionRequest request;

  mutable bool is_collision;

  const NarrowPhaseSolver* nsolver;
};

template<typename S1, typename S2, typename NarrowPhaseSolver>
class ShapeDistanceTraversalNode : public DistanceTraversalNodeBase
{
public:
  ShapeDistanceTraversalNode() : DistanceTraversalNodeBase()
  {
    model1 = NULL;
    model2 = NULL;

    nsolver = NULL;
  }

  FCL_REAL BVTesting(int, int) const
  {
    return -1; // should not be used 
  }

  void leafTesting(int, int) const
  {
    is_collision = !nsolver->shapeDistance(*model1, tf1, *model2, tf2, &min_distance);
  }

  const S1* model1;
  const S2* model2;

  mutable FCL_REAL min_distance;
  mutable Vec3f p1;
  mutable Vec3f p2;

  mutable bool is_collision;

  const NarrowPhaseSolver* nsolver;
};


}

#endif

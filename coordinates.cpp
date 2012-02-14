/**
 * Operators for Coordinates
 *
 * Copyright 2011 Bruce Ide
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "coordinates.h"

bool operator==(LatlongInterface &lhs, LatlongInterface &rhs)
{
  return (lhs.getLat() == rhs.getLat()) && (lhs.getLong() == rhs.getLong()) && (lhs.getAlt() && rhs.getAlt());
}

bool operator==(Ecef &lhs, Ecef &rhs)
{
  return (lhs.getX() == rhs.getX()) && (lhs.getY() == rhs.getY()) && (lhs.getZ() && rhs.getZ());
}

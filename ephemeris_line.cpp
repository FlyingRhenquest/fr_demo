/**
 * Operators for ephemeris line
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

#include "ephemeris_line.h"
#include <math.h>

bool operator==(EphemerisLine &lhs, EphemerisLine &rhs)
{
  return (lhs.getPosition() == rhs.getPosition()) && (lhs.getDx() == rhs.getDx()) && (lhs.getDy() == rhs.getDy()) && (lhs.getDz() == rhs.getDz());
}

bool operator!=(EphemerisLine &lhs, EphemerisLine &rhs)
{
  return !(lhs == rhs);
}

/**
 * eqalish takes two refernces and a tolerance and returns true if
 * nothing is not equal below those two tolerances. Because unless
 * you're comparing exactly the same line, two doubles will probably
 * never actually be equal, even if they "look" equal.
 */

bool equalish(double tolerance, EphemerisLine &lhs, EphemerisLine &rhs)
{
  double x, y, z, dx, dy, dz;
  x = fabs(lhs.getPosition().getX() - rhs.getPosition().getX());
  y = fabs(lhs.getPosition().getY() - rhs.getPosition().getY());
  z = fabs(lhs.getPosition().getZ() - rhs.getPosition().getZ());
  dx = fabs(lhs.getDx() - rhs.getDx());
  dy = fabs(lhs.getDy() - rhs.getDy());
  dz = fabs(lhs.getDz() - rhs.getDz());

  /* // could be handy if writing tests
     std::cout << "Tolerances: " << std::endl;
     std::cout << "         x: " << x << std::endl;
     std::cout << "         y: " << y << std::endl;
     std::cout << "         z: " << z << std::endl;
     std::cout << "        dx: " << dx << std::endl;
     std::cout << "        dy: " << dy << std::endl;
     std::cout << "        dz: " << dz << std::endl;
  */

  return (x < tolerance && y < tolerance && z < tolerance &&
          dx < tolerance & dy < tolerance && dz < tolerance);
}

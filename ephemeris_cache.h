/**
 * This object caches ephemeris lines and can be called upon to
 * retrieve them for a given time. This object will delete
 * ephemeris lines put into it when it is deleted, so there's 
 * no need to track that yourself.
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

#ifndef _H_EPHEMERIS_CACHE
#define _H_EPHEMERIS_CACHE

#include "coordinates.h"
#include "ephemeris_line.h"
#include <map>
#include <string>
#include <time.h>
#include <iostream>
#include <vector>
#include "time_tree.h"


class EphemerisCache {

  class SatelliteTreeDeallocator {
  public:

    void operator()(EphemerisLine *value) { delete value;}

  };
  
  
  typedef TimeTree<double, EphemerisLine *, SatelliteTreeDeallocator> SatelliteTree;
  typedef std::map<std::string, SatelliteTree *> SatelliteMap;
  typedef std::map<std::string, double> IntervalMap;
  SatelliteMap satellites;
  /**
   * intervals tracks coordinate intervals. If you request a time
   * later than the latest coordinate + the interval for that satellite,
   * "not found" should be returned.
   */
  IntervalMap intervals; 

  /**
   * (re)calculates the average data interval of your ephemeris points.
   * This is really only to provide "not found" points past the end
   * of the file, but I guess that's fairly useful information. I
   * could add a flag to not do this if I don't query that space
   * that often. Will have to see...
   */

  double calcInterval(const std::string &satellite) {
    SatelliteMap::iterator iter = satellites.find(satellite);
    double retval = 0.0;
    if (iter != satellites.end()) {
      double timeAccum = 0.0;
      double lastTime = 0.0;
      long nelems = 0;
      SatelliteTree *tree = iter->second;
      SatelliteTree::RangeType treeRange;
      SatelliteTree::RangeType::iterator rangeIter;
      tree->findRange(treeRange, tree->begin(), tree->end());
      rangeIter = treeRange.begin();
      while(rangeIter != treeRange.end()) {
        if (lastTime == 0.0) {
          lastTime = (*rangeIter)->first;
          rangeIter++;
          continue;
        }
        nelems++;
        timeAccum += (*rangeIter)->first - lastTime;
        lastTime = (*rangeIter)->first;
        rangeIter++;
      }
      retval = timeAccum / (double) nelems;
      intervals[satellite] = retval;
    }
    return retval;
  }

public:
  EphemerisLine *NOT_FOUND;

  EphemerisCache() { NOT_FOUND = (EphemerisLine *) NULL; }
  ~EphemerisCache() 
  {
    SatelliteMap::iterator sats = satellites.begin();
    while (sats != satellites.end()) {
      delete sats->second;
      sats++;
    }
  }

  /**
   * Note that if you want this function to work correctly,
   * you'll need to set your time in your EphemerisLine
   */

  void add(const std::string &satellite, EphemerisLine *line)
  {
    SatelliteMap::iterator sat = satellites.find(satellite);
    SatelliteTree *found = NULL;
    if (sat == satellites.end()) {
      SatelliteTree *newSat = new SatelliteTree(NOT_FOUND);
      satellites[satellite] = newSat;
      intervals[satellite] = 0.0;
      found = newSat;
    } else {
      found = sat->second;
    }
    found->add(line->getTime(), line);
  }

  /**
   * Get gets the EphemerisLine for the satellite for a given
   * time. Returns NULL if not found.
   */

  EphemerisLine *get(const std::string &satellite, double time)
  {
    EphemerisLine *retval = (EphemerisLine *) NULL;
    SatelliteMap::iterator sat = satellites.find(satellite);
    SatelliteTree *found = NULL;
    if (sat != satellites.end()) {
      found = sat->second;
      double dataInterval = 0.0;
      /*
       * We really only need to know the data interval if we're
       * querying past the end of the data points we have.
       */
      if (time > found->end()) {
        IntervalMap::iterator intr = intervals.find(satellite);
        if (intr == intervals.end() || intr->second == 0.0) {
          dataInterval = calcInterval(satellite);
        } else {
          dataInterval = intr->second;
        }
        retval = found->find(time);
        if (time > retval->getTime() + dataInterval) {
          retval = (EphemerisLine *) NULL;
        }
      } else {
        retval = found->find(time);
      }
    }
    
    return retval;
  }

  /**
   * Allow for the explicit query of the data point interval for
   * your satellite. I've never seen this NOT be regular times,
   * but it does average. If you collect data points at weird-ass
   * intervals, you may need to compute a deviation. Or maybe this
   * information won't do you much good. Or both...
   */

  double getDataInterval(std::string &satellite)
  {
    IntervalMap::iterator found = intervals.find(satellite);
    double retval = 0.0;
    if (found == intervals.end() || found->second == 0.0) {
      retval = calcInterval(satellite);
    } else {
      retval = found->second;
    }
    return retval;
  }

  /**
   * Put list of satellite names into a std::vector<std::string>
   */

  void satelliteNames(std::vector<std::string> &names)
  {
    SatelliteMap::iterator it = satellites.begin();
    while(it != satellites.end()) {
      names.push_back(it->first);
      it++;
    }
  }

};

#endif
